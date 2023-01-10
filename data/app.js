"use strict";

function Cambia_Tema() {
    const cambiador = document.querySelector('.tema-visual');
    document.body.classList.toggle('tema-claro');
    document.body.classList.toggle('tema-oscuro');
    document.body.style.transition = "1s ease"; //indicado por openAi
    const className = document.body.className;
    if(className == 'tema-claro') {
        document.getElementById("temVis").innerHTML = 'Oscuro';
    }
    else if(className == 'tema-oscuro') {
        document.getElementById("temVis").innerHTML = 'Claro';
    }
}

function botonSalir() {
    var sHTTP = new XMLHttpRequest();
    sHTTP.open("GET", "no-permitido", true);
    sHTTP.send();
    setTimeout(function(){window.open("autenticar.html", "_self");}, 1000)
}

function resPantalla() {
    return window.innerWidth;
}

