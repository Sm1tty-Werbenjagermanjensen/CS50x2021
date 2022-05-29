console.log(shirts);
// Get id/key of shirt
const key = sessionStorage.getItem("shirt");
console.log(shirts[key]);
// If no shirt was selected, redirect to homepage
if (key == null)
{
    window.location.replace("index.html");
}
// Declare item img path, name, and price
const img = imgDirectory + shirts[key][0] + jpg;
const name = shirts[key][1];
const price = shirts[key][2];
// Set page title
document.querySelector("title").innerHTML = name;
// Set product image to item
const productImage = document.querySelector(".product").children[0].attributes;
productImage[0].value = img;
productImage[1].value = name;
// Set product info
const productInfo = document.querySelector(".info").children;
productInfo[0].innerHTML = "ID: " + key;
productInfo[1].innerHTML = name;
productInfo[2].innerHTML = "$" + price;

//initialize Size Menu
// dropdown-item template
const sizeTemplate = '<a class="dropdown-item" onclick="selectSize(this)"></a>';
// Get sizeMenu
const sizeMenu = document.querySelector(".dropdown-menu");
// Add sizes (sizes to shirts values array index: S = 3, M = 4, L = 5)
for (var i = sizeIndex.S; i <= sizeIndex.L; i++)
{
    var sizeOption;
    sizeMenu.innerHTML += sizeTemplate;
    sizeOption = sizeMenu.children[i - sizeIndex.S];
    sizeOption.innerHTML = indexSize[i];
    // If size is out of stock, or already added to cart: disable option
    if (parseInt(shirts[key][i]) == 0 || sessionStorage.getItem(key + sizeOption.innerHTML) != null)
    {
        disableSizeOption(sizeOption);
    }
}
function disableSizeOption(el) {
    el.attributes[0].value += " disabled";
}
// Selects Size (called upon clicking size)
var size; // Chosen size
var stock; // Chosen stock
const invalidPrompt = document.querySelector(".invalid-item-add"); // Used to prompt valid input
function resetQuantity () {
    quantity.innerHTML = "0";
}
function selectSize(el) {
    resetQuantity();   
    // Set size variable and display it on dropdown menu
    size = el.innerHTML;
    quantity.innerHTML = "1";
    displaySize();
 }
function displaySize() {
    const sizeDisplay = document.querySelector(".dropdown-toggle");
    if (size == null) 
    {
        sizeDisplay.innerHTML = "Select Size";
    }
    else 
    {
        sizeDisplay.innerHTML = size;
    }
}
// Upon clicking "Add to Cart", put selection in global storage
function addCart() 
{
       // If input is valid, send selection to session stoarge
    if (!invalidInput(true)) 
    {
        // Update session storage
        selectionUpdate(key, size, quantity);
        // Increment nCart
        nCartUpdate();
        // Disable size option that was chosen
        disableSizeOption(sizeMenu.children[sizeIndex[size] - sizeIndex.S]);
        // Reset item page
        size = null;
        displaySize();
        resetQuantity();
        // Update cart badge
        cartBadge();
    }    
}
// Declare quantity
const quantity = document.querySelector(".quantity");
// On click of quantity +/-
function quantityChangeCheck(el) {
    // If input invalid, exit
    if (invalidInput(false)) {return;}
    // try to change quantity
    changeQuantity(el);
}
// Argument is true on add to cart and otherwise false
function invalidInput(cart)
{
     // If no size is chosen
     if (size == null) 
    {
        invalidPrompt.innerHTML = "No size selected.";
        return true;
    }
    // If user clicked add to cart, size is chosen but no quantity
    else if (cart && size != null && quantity.innerHTML == "0")
    {
        invalidPrompt.innerHTML = "No quantity selected.";
        return true;
    }
    // else clear prompt and return as being valid
    else
    {
        invalidPrompt.innerHTML = ""
        return false;
    }
}
