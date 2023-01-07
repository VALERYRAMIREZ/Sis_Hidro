function botonSalir() {
    var sHTTP = new XMLHttpRequest();
    sHTTP.open("GET", "no-permitido", true);
    sHTTP.send();
    setTimeout(function(){window.open("autenticar", "_self");}, 1000)
}

function resPantalla() {
    return window.innerWidth;
}
