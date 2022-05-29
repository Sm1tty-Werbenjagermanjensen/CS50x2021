/* Dictionary of shirts 
   Values = [
            0 = "product img  name", 
            1 = "title", 
            2 = "price", 
            |# of SIZES|
            3 = S
            4 = M
            5 = L
            ] */
var shirts = {
    whySoStylish: ["why-so-stylish", "Why So Stylish?", "399.99", "5", "10", "15"],
    swamp: ["swamp", "Swamp", "21.99", "4", "5", "6"],
    fartLoading: ["fart-loading", "Fart Loading...", "21.99", "10", "0", "0"],
    suhDude: ["suh-dude", "Suh Dude", "0.00 (PLEASE TAKE IT)", "50", "99", "90"],
    super: ["super", "(Trust bro, it's real)", "977.99", "0", "0", "5"],
    noWire: ["no-wire", "Not Wearing a Wire", "21.99", "20", "56", "11"] 
};

// Shirt image directory and file format suffix
const imgDirectory = "images/shirts/";
const jpg = ".jpg"

// Size indecies in values array
const sizeIndex = {
    S:3,
    M:4, 
    L:5
}
// Dictionary for shirts values index to size value
const indexSize = {
    3:"S",
    4:"M",
    5:"L"
};
// Declare number of items in cart
var nCart;
// Updates nCart
function nCartUpdate() {
    nCart = sessionStorage.length - 1;
    if (sessionStorage.getItem("shirt") == null)
        {
            nCart = sessionStorage.length;
        }
}

// Cart badge
const cartLink = document.querySelector('a[href="cart.html"]');
const cartTemplate = 'Cart<span class="badge badge-primary"></span>';
// Updates cart badge in nav bar
function cartBadge() {
    if (nCart == 0)
    {
        cartLink.innerHTML = "Cart";
    }
    else
    {
        cartLink.innerHTML = cartTemplate;
        cartLink.children[0].innerHTML = nCart;
    }
}

// On click of showcase, call function to select shirt
function select(el) {
    // Place key/id for shirt in session storage
    sessionStorage.setItem("shirt", el.attributes[2].value);
}
// Declare quantity +/- buttons
const quantityButtons = document.querySelectorAll(".quantity-change");
// Changes quantity when +/- clicked (within stock range)
function changeQuantity(el) {
    // Declare operation value
    var op;
    const opClicked = el.classList[3];
    if (opClicked == "plus")
    {
        op = 1;
    }
    else 
    {
        op = -1;
    }
    // Declare current quantity (before click), calculate new quantity, and declare number of shirts in that size which are in stock
    const curQuantity = parseInt(quantity.innerHTML);
    const newQuantity = curQuantity + op;
    const stock = shirts[key][sizeIndex[size]];
    // If a size is chosen and the new quantity is in the limits of its stock, then change quantity to new quantity
    if (newQuantity >= 1 && newQuantity <= stock)   
    {
        quantity.innerHTML = newQuantity;
    }
}

// Update session storage item
function selectionUpdate (key, size, quantity) {
        var selection = [];
        selection[0] = key;
        selection[1] = size;
        selection[2] = quantity.innerHTML;
        // Store selection at that cart number
        sessionStorage.setItem(key + size, JSON.stringify(selection));

}

// initialize nCart and cart badge
nCartUpdate();
cartBadge();
