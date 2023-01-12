"use strict";
//localStorage.clear();
//Aplica_Estado_Inicial();

var estados = document.getElementById("temVis");
estados.addEventListener("click", function() {
    console.log("Entrando a acción de cambiador de tema.");
    let tTema;
    const cambiador = document.querySelector('.tema-visual');
    document.body.classList.toggle('tema-claro');
    document.body.classList.toggle('tema-oscuro');
    document.body.style.transition = "1s ease"; //indicado por openAi
    localStorage.setItem("tema", document.getElementsByTagName("body")[0].className);
    const className = document.body.className;
    if(className == 'tema-claro') {
        tTema = 'Oscuro';
    }
    else if(className == 'tema-oscuro') {
        tTema = 'Claro';
    }
    document.getElementById("temVis").innerHTML = tTema;
    localStorage.setItem("textoTema", tTema);
    console.log(localStorage.getItem("textoTema"));
});

var bloqueado = document.getElementById("bloquearSistema");
bloqueado.addEventListener("click", function() {
    var sHTTP = new XMLHttpRequest();
    sHTTP.open("GET", "no-permitido", true);
    sHTTP.send();
    setTimeout(function(){window.open("autenticar.html", "_self");}, 1000);    
});

function resPantalla() {
    return window.innerWidth;
}

function Aplica_Estado_Inicial() {
    if(localStorage.getItem("primeraCarga") === null) {
        console.log("Clases del body: " + document.body.classList);
        let claseBody = document.body.classList= "tema-claro";
        let textoBoton = document.getElementById("temVis").innerHTML = "Oscuro";
        localStorage.setItem("primeraCarga", "true");
        localStorage.setItem("tema", claseBody);
        localStorage.setItem("textoTema", textoBoton);
        console.log("Primera carga hecha");
    }
    else if((localStorage.getItem("primeraCarga") === "true") && !(localStorage.tema === undefined)) {
        document.body.className = localStorage.getItem("tema");
        document.getElementById("temVis").innerHTML = localStorage.getItem("textoTema");
        console.log("Ya no es la primera carga");
    }
    console.log("primeraCarga: " + localStorage.getItem("primeraCarga"));
    console.log("tema: " + localStorage.getItem("tema"));
    console.log("texto: " + localStorage.getItem("textoTema"));
    /*if(!(localStorage.tema === undefined)) {
        document.body.className = localStorage.getItem("tema");
        document.getElementById("temVis").innerHTML = localStorage.getItem("textoTema");
    }*/
    return;
}