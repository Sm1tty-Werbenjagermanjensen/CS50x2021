// Get Title of page
var page = document.querySelector(".active").attributes[0].value;
// Declare showcase template
const template = '<li class="list-inline-item"><a href="item.html" onclick="select(this)" id=""><img src="" alt""><h3></h3><h4></h4></a></li>';
// Get Inline List
const listInline = document.querySelector(".list-inline");
// Declare number of shirts
var nShirts = Object.keys(shirts).length;
// Create number of needed templates
var nShowcases;
if (page == "shop.html")
{
    nShowcases = nShirts;
}
else 
{
    nShowcases = 3;
}
listInline.innerHTML = template.repeat(nShowcases);
// Get array of product spots
const showcases = document.querySelectorAll(".list-inline-item");
// Initialize Counter, showcase var, and shirt entries arr
var counter = 0;
var showcase;
var entries = Object.entries(shirts);
// If on item page, remove selected shirt from entries
if (page == "item.html")
{
    var tempShirts= {};
    Object.assign(tempShirts, shirts);
    var selected = sessionStorage.getItem("shirt");
    delete tempShirts[selected];
    entries = Object.entries(tempShirts);
    nShirts--;
}
// Get 3 random shirts to showcase if use is on home page
if (page == "index.html" || "item.html")
{
    var ran;
    for (var i = 0; i < (nShirts - nShowcases); i++)
    {
        ran = Math.floor(Math.random() * entries.length);
        entries.splice(ran, 1);
    }
}
// Set values of each Showcase
for (const [index, values] of entries) 
{
    showcase = showcases[counter].firstChild;
    // Set id
    showcase.attributes[2].value = index;
    // Set img, title, & price
    var value;
    for (var i = 0; i < showcase.children.length; i++) {
        if (i == 0) 
        {
            for (var j = 0; j < 2; j++) 
                {
                    value = values[j];
                    if (j == 0) {value = imgDirectory + value + jpg};
                    showcase.children[i].attributes[j].value = value;
                }
        }
        else 
        {
            value = values[i]
            if (i == 2) value = "$" + value;
            showcase.children[i].innerHTML = value;
        }
    }
    counter++;
}
// On click of showcase, call function to select shirt
function select(el) {
    // Place key/id for shirt in session storage
    sessionStorage.setItem("shirt", el.attributes[2].value);
}
