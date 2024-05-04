"use strict";
//import fs from 'fs';
//localStorage.clear();
//Aplica_Estado_Inicial();

if(document.getElementById("btnC1") != null) {
    var inicialBotonBg = window.getComputedStyle(document.getElementById("btnC1")).backgroundColor;
};

/*------------Funcionamiento del cambio de temas------------------------*/
/*-------------------de la aplicación-----------------------------------*/

var estados = document.getElementById("temVis");
estados.addEventListener("click", function() {
    let tTema;
    const cambiador = document.querySelector('.tema-visual');
    document.body.classList.toggle('tema-claro');
    document.body.classList.toggle('tema-oscuro');
    document.body.style.transition = "1s ease"; //indicado por openAi
    if(document.getElementById("btnC1") != null) {
        inicialBotonBg = window.getComputedStyle(document.getElementById("btnC1")).backgroundColor;
    };
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

/*----------------Funcionamiento de los cambios de estilo-------------------*/
/*------------------en los botones de control del sistema-------------------*/
/*--------------------------------de bombeo---------------------------------*/

var botonControl = document.querySelectorAll("a");
botonControl.forEach(enlace => {
    enlace.addEventListener("click", function(event) {
        let clickadas = 1; 
        console.log("Evento 'event': ");
        console.log(event.target);
        let solHref = event.target.href;
        console.log("href del botón: " + solHref);
    });
});

if(document.getElementById("forma") != null) {              // Si encuentra la forma con la id igual a "forma",
    var elementoForma = document.getElementById("forma");   // extrae los valores completos.
    elementoForma.addEventListener("submit", function(e) {
        e.preventDefault();
        var datosForma = {};
        var formaDatos = new FormData(elementoForma);
        for(var [k,v] of formaDatos) {
            datosForma[k] = v;
        }
        console.log(datosForma);
        var datosJson = JSON.stringify(datosForma)
        console.log(datosJson);
        var config = new Blob([datosJson], {type: 'application/json'});
        fetch("forma-dato", {
            method: "POST",
            headers: {
                'Content-Type': 'application/json',
            },
            body: datosJson
        });
        /*fs.writeFile('config.json', datosJson, 'utf-8', (err) => {
            if(err) {
                throw err;
            }
            console.log("Archivo guardado correctamente");
        });*/
        console.log(config);
        /*var envioForma = new XMLHttpRequest();
        envioForma.open("POST", "forma-dato", true);
        envioForma.send(config);*/
    });
}
function setBackgroundPositionY(y) {
    return `background-position: 0px ${y}%;`;
  }

/*function Aplica_Estado_Inicial() {
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
    }
    return;
}*/