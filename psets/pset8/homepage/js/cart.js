// Remove shirt selection variable from session storage
// This allows cart items to be iterated over by iterating session storage
sessionStorage.removeItem("shirt");
nCartUpdate;
// Cart item template
const itemTemplate = '<li class="list-group-item cart-item"><a role="button" class="float-right btn bg-none remove-from-cart" onclick="removeFromCart(this)"><i class="fa fa-window-close" aria-hidden="true"></i></a><a href="item.html" onclick="select(this)" class=""><div class="row"><div class="col-0"><img src="" alt=""></div><div class="col-0"><h3></h3><h4></h4></div></div></a><h4>Size: </h4><div class="row quantity-menu "><div class="col-0"><a role="button" class=" btn bg-none quantity-change minus" onclick="changeQuantityItem(this)"><i class="fa fa-minus"></i></a></div><div class="col-0"><p class="quantity"></p></div><div class="col-0"><a class="btn bg-none quantity-change plus" onclick="changeQuantityItem(this)"><i class="fa fa-plus"></i></a></div></li>'
const noItemsTemplate = '<li class="list-group-item empty-cart"><h3>No items in cart.</h3></li>'
// Get cart display list
const cartDisplay = document.querySelector(".cart");
// Declare cart contents arr
var cart = [];
// Declare cart key var
var cartKey;
// Declare key and size for changeQuantity(el)
var key;
var size;
// initialize cart
cartUpdate();

function cartUpdate()
{
    // Reset cart display
    cartDisplay.innerHTML = "";
    // Reset cart total
    var total = 0;
    // If cart is empty, display such and return
    if (nCart == 0)
    {
        cartDisplay.innerHTML = noItemsTemplate;
        return;
    }
    // Fill cart contents and display with items
    for (var i = 0; i < nCart; i++)
    {

        cartDisplay.innerHTML += itemTemplate;
        cartKey = sessionStorage.key(i);
        cart[i] = JSON.parse(sessionStorage.getItem(cartKey));
        // Declare current item, shirts key, and shirt name
        var item = cartDisplay.children[i]
        var key = cart[i][0];
        var name = shirts[key][1];
        // Set link id
        var link = item.children[1];
        link.attributes[2].value = key
        // Set image values
        var img = link.children[0].children[0].children[0]
        img.attributes[0].value = imgDirectory + shirts[key][0] + jpg;
        img.attributes[1].value = name;
        // Set name
        var heading = link.children[0].children[1]
        heading.children[0].innerHTML = name;
        // Set size
        item.children[2].innerHTML += cart[i][1];
        // Set key into quantity menu class 
        var quantityMenu = item.children[3];
        quantityMenu.attributes[0].value += key;
        // Declare quantity value element
        var quantityValue = quantityMenu.children[1].children[0];
        // Declare quantity
        const n = parseInt(cart[i][2]);
        // Set quantity value;
        quantityValue.innerHTML= n;
        // Declare price
        const price = parseInt(shirts[key][2]);
        // Set price on item
        var pricePrefix = "$";
        if (n > 1)
        {
            pricePrefix = n + " x $";
        }
        heading.children[1].innerHTML = pricePrefix + price;
        // Increment total
        total += n * price;
        // Set total
        document.querySelector("#total").innerHTML = "Total: $" + total;
    }
}

// Called to change quantity of item
function changeQuantityItem(el) {
    const menuRoot = el.parentElement.parentElement 
    key = menuRoot.classList[2];
    size = menuRoot.previousSibling.innerHTML.substr(6);
    quantity = menuRoot.children[1].children[0];
    changeQuantity(el);
    // Update session storage
    selectionUpdate(key, size, quantity); 
    // Update cart on page
    cartUpdate();
}

// Called to remove item from cart
function removeFromCart(el) {
    // Get shirt id, size
    const id = el.nextSibling.attributes[2].value;
    const size = el.nextSibling.nextSibling.innerHTML.substr(6);
    // Remove item from session storage
    sessionStorage.removeItem(id + size);
    // Decrement nCart
    nCartUpdate();
    // Update cart and badge on page
    cartUpdate();
    cartBadge();
}

// Called to "Check Out" 
function checkOut() {
    document.querySelector("body").innerHTML = '<video autoplay loop playsinline poster="images/mfw.jpg"><source src="videos/my-man-ricky.mp4" type="video/mp4">';
    document.querySelector("video").play();
}
