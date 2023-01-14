"use strict";
//localStorage.clear();
//Aplica_Estado_Inicial();

/*------------Funcionamiento del cambio de temas------------------------*/
/*-------------------de la aplicación-----------------------------------*/

var estados = document.getElementById("temVis");
estados.addEventListener("click", function() {
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

/*----------------Funcionamiento de los cambios de estilo-------------------*/
/*------------------en los botones de control del sistema-------------------*/
/*--------------------------------de bombeo---------------------------------*/

var botonControl = document.querySelectorAll("a");
botonControl.forEach(enlace => {
    enlace.addEventListener("click", function(e) {
        console.log("Evento e: ");
        console.log(e);
        let solHref = e.target.href;
        //console.log("href del botón: " + solHref);
        let regex = /\/(\w+)$/;
        let funcSistema = solHref.match(regex)[0];
        //console.log(typeof(funcSistema));
        //console.log("Dirección parcial: " + funcSistema);
        switch(funcSistema) {
            case '/apagar':
            {
                console.log("APAGAR");
            }
            break;
            case "/encender":
            {
                console.log("ENCENDER");
            }
            break;
            case "/auto":
            {
                console.log("AUTO");
            }
            break;
            case "/manual":
            {
                console.log("MANUAL");
            }
            break;
            default:
            {
                console.log("Sin opción para funcSistema = " + funcSistema);
            }
            break;
        }
    });
    return;   
});
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