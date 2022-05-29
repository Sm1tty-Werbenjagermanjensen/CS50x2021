import os

from datetime import datetime

from decimal import Decimal, ROUND_DOWN

import sqlalchemy as db
from sqlalchemy.orm import sessionmaker
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.sql import func

from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session

from tempfile import mkdtemp

from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)
app.debug = True

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure sql alchemy to use SQLite database
Base = declarative_base()
# Define connection object
engine = db.create_engine("sqlite:///finance.db", connect_args={"check_same_thread": False})
# Create model
class users(Base):
    __tablename__ = 'users'

    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String, nullable=False, unique=True)
    hash = db.Column(db.String, nullable=False)
    cash = db.Column(db.Float, nullable=False, default="10000.00")

class transactions(Base):
    __tablename__ = 'transactions'

    id = db.Column(db.Integer, primary_key=True)
    user_id = db.Column(db.Integer, nullable=False)
    symbol = db.Column(db.String, nullable=False)
    shares = db.Column(db.Integer, nullable=False)
    price = db.Column(db.Float, nullable=False)
    time = db.Column(db.DateTime(timezone=True), server_default=func.now())

class holds(Base):
    __tablename__ = 'holds'

    id = db.Column(db.Integer, primary_key=True)
    user_id = db.Column(db.Integer, nullable=False)
    symbol = db.Column(db.String, nullable=False)
    shares = db.Column(db.Integer, nullable=False)

Base.metadata.create_all(engine)
# Create a session object
Session = sessionmaker()
Session.configure(bind=engine)
DB = Session()

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Get user's cash balance
    cash = querycash()
    
    # Declare total
    total = cash

    # Get all holds of user
    user_holds = userholds()
   
    # lookup each stock and add additional info to hold
    for hold in user_holds:
        
        # lookup
        quote = lookup(hold.symbol)
        # Add stock name
        hold.name = quote["name"]
        # Get price and calculate hold total
        hold_price = quote["price"]
        hold_total = hold.shares * float(hold_price)
        # Add price and hold total
        hold.price = usd(hold_price)
        hold.total = usd(hold_total)

        # Add hold total to net total
        total += hold_total

    return render_template("index.html", holds=user_holds, cash=usd(cash), total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    
    if request.method == "POST":

        # Process transaction as buy
        error = process_transaction(True, request.form.get("symbol"),request.form.get("shares"))

        # if error returned in process, call apology
        if error:
            return apology(error, 400)

        # Redirect to index
        flash("Bought!")
        return redirect("/")


    if request.method == "GET":

        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Get user's transactions
    user_transactions = DB.query(transactions).filter(transactions.user_id==session["user_id"]).all()

    # Render template
    return render_template("history.html", transactions=user_transactions)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        result = queryusername(request.form.get("username"))
        # Ensure username exists and password is correct
        if len(result) != 1 or not check_password_hash(result[0].hash, request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        login(result[0])

        # Redirect user to home page
        flash(f"Welcome back, {result[0].username}!") 
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        
        # Declare symbol
        symbol = request.form.get("symbol")

        # Ensure symbol given 
        if not symbol:
            return apology("missing symbol", 400)
        
        # Lookup quote
        quote = lookup(symbol)
        
        # Ensure symbol is valid
        if not quote:
            return apology("invalid symbol", 400)

        return render_template("quoted.html", name=quote["name"], symbol=quote["symbol"], price=quote["price"])
        

    if request.method == "GET":
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
# Forget any user_id
    session.clear()

# User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
       
        # Declare user input
        uname = request.form.get("username")
        pword = request.form.get("password")

        # Ensure username was submitted
        if not uname:
            return apology("must provide username", 403)

        # Ensure password was submitted
        if not pword:
            return apology("must provide password", 403)

        # Ensure password was confirmed
        if request.form.get("confirmation") != pword:
            return apology("failed to confirm password", 403)
       
        # Ensure username is available
        result = queryusername(uname)
        if len(result) != 0:
            return apology("username unavailable", 403)

        # Add user to database
        user = users(username=uname, hash=generate_password_hash(pword))
        DB.add(user)
        DB.commit()

        # Log user in
        login(user)

        # Redirect user to Index
        flash("Registered!")
        return redirect("/")

    if request.method == "GET":
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        # Process transaction NOT as buy (i.e sale)
        error = process_transaction(False, request.form.get("symbol"), request.form.get("shares"))

        # if error returned in process, call apology
        if error:
            return apology(error, 400)

        # Redirect to index
        flash("Sold!")
        return redirect("/")


    if request.method == "GET":
        # Get all user holds
        user_holds = userholds()

        # Render template
        return render_template("sell.html", holds=user_holds)

@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    """Deposit cash into user account"""
    if request.method == "POST":

        # Ensure amount is given and is float rounded to nearest cent
        try:
            amount = float(Decimal(request.form.get("cash")).quantize(Decimal('.01'), rounding=ROUND_DOWN))
        except:
            return apology("invalid amount", 400)
       
        # Ensure amount is positive

        if not amount > 0:
            return apology("invalid amount", 400)

        # Add cash to user's account
        updatecash(amount, False)

        # Redirect to index
        flash(f"Successfully Deposited: {usd(amount)}")
        return redirect("/")


    if request.method == "GET":

        # Render template
        return render_template("deposit.html")

@app.route("/changepassword", methods=["GET", "POST"])
@login_required
def changepassword():
    """Change user's password"""
    if request.method == "POST":
        
        # Declare inputs and ensure they are given

        current = request.form.get("current")
        if not current:
            return apology("must provide current password", 400)
        
        new = request.form.get("new")
        if not new:
            return apology("must provide new password", 400)
        
        confirmation = request.form.get("confirmation")
        if not confirmation:
            return apology("must provide password confirmation", 400)

        # Get user data
        user = queryuser()

        # Ensure current password is correct

        if not check_password_hash(user.hash, current):
            return apology("current password incorrect")

        # Ensure new password and confirmation match

        if new != confirmation:
            return apology("new password does not match confirmation", 400)

        # Change password to new password

        user.hash = generate_password_hash(new) 
        DB.commit()

        # Redirect to index
        flash("Password change successful!")
        return redirect("/")
   
    if request.method == "GET":
         
        # Render template
        return render_template("changepassword.html")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)

def login(user):
    
    #log in user given user object
    session["user_id"] = user.id

def queryuser():
    return DB.execute(db.select(users).filter(users.id==session["user_id"])).scalar_one()

def queryusername(input_username):
    query = db.select(users).where(users.username == input_username)
    return engine.execute(query).fetchall()

def querycash():
    query = db.select(users.cash).where(users.id == session["user_id"])
    result = engine.execute(query).fetchall()
    return result[0].cash

def updatecash(total, buy):
    user = queryuser()
    cash = user.cash
    if buy:
        cash -= total
    else:
        cash += total

    user.cash = cash
    DB.commit()
    return

def userholds():
    # Return stock holds of user
    return DB.query(holds).filter(holds.user_id==session["user_id"]).all()

def sharesheld(symbol):
    # Return user's hold in stock
    return DB.query(holds).filter((holds.user_id==session["user_id"]) & (holds.symbol==symbol)).one()

def updatehold(symbol, shares):

    # Query hold or create hold if no shares are already held in stock
    try:
        hold = sharesheld(symbol)
    except: 
        hold = holds(
            user_id=session["user_id"],
            symbol=symbol,
            shares=0)
        DB.add(hold)
        DB.flush()
    # Add or subtract shares
    hold.shares += shares
    # If 0 shares left, remove hold entry
    if hold.shares == 0:
        DB.delete(hold)

    DB.commit()
    return
        
def process_transaction(buy, symbol_input, shares_input):
    
    # Validate symbol 
    symbol = symbol_input.upper()
        
    # Lookup symbol
    stock = lookup(symbol)

    # Ensure symbol given and exists
    if not stock:
        return "invalid symbol"

    # Validate shares    
    # Ensure shares is an integer
    try:
        shares = int(shares_input)
    except:
        return "invalid shares"

    # Ensure 1 or more shares selected
    if shares < 1:
        return "invalid shares"
       
    # Find total price of purchases
    price = stock["price"]
    total = shares * price

    if buy:
        # Ensure sufficient funds
        if total > querycash():
            return "can't afford"
    else:
        try:
            # Ensure shares selected don't exceed amount owned
            if shares > sharesheld(symbol).shares:
                return "too many shares"
        except:
            # If exception raised, symbol is not a stock in which the user holds
            return "stock not held"
 
        # Negate shares (as they are being sold not bought)
        shares = -shares

    # Create transaction entry
    transaction = transactions(
        user_id=session["user_id"],
        symbol=symbol,
        shares=shares,
        price=price)
    
    DB.add(transaction)
    DB.commit()

    # Update user's stock holds
    updatehold(symbol, shares)
    
    # Update user cash
    updatecash(total, buy)

    if symbol == "GME":
        if buy:
            flash("TO THE MOON!")
        else:
            flash("coward.")
    return

# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
