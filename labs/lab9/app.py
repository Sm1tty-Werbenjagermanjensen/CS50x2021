import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

# Declare max number of days in each month 
nDays = [31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        
        # Declare possible form values
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")
        id = request.form.get("id")
        
        # Add birthday on appropriate input
        # Reject a name with ';' to avoid sql code injection
        if name and ';' not in name and int(month) - 1 in range(12) and int(day) <= nDays[int(month) - 1]:
            db.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?);", name, month, day)
        
        # Remove a birthday on appropriate input 
        if id:
            db.execute("DELETE FROM birthdays WHERE id = ?;", id)
        return redirect("/")

    else:
        
        birthdays = db.execute("SELECT * FROM birthdays;")
        return render_template("index.html", birthdays=birthdays)


