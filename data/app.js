function botonSalir() {
    var sHTTP = new XMLHttpRequest();
    sHTTP.open("GET", "no-permitido", true);
    sHTTP.send();
    setTimeout(function(){window.open("autenticar", "_self");}, 1000);
}

function resPantalla() {
    return window.innerWidth;
}"use strict";

const cambiador = document.querySelector('.tema-visual');
cambiador.addEventListener('click',function() {
    document.body.classList.toggle('tema-claro');
    document.body.classList.toggle('tema-oscuro');

    const className = document.body.className;
    console.log("Las clases son: " + className);
    if(className == 'tema-claro') {
        console.log('Contexto actual: ' + this);
        console.log('HTML interno actual del botón: ' + document.getElementById("temVis"));
        document.getElementById("temVis").innerHTML = 'Oscuro';
    }
    else if(className == 'tema-oscuro') {
        console.log('Contexto de texto actual: ' + this.textContext);
        document.getElementById("temVis").innerHTML = 'Claro';
    }
    else {

    }
    console.log('Clase actual: ' + className);
    console.log('Contexto de texto: ' + this.textContext);
});

