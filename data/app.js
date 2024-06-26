"use strict";
//import fs from 'fs';
//localStorage.clear();
//Aplica_Estado_Inicial();

// Funciones para cargar el tema de la página antes de que se renderice.

function Carga_Tema()
{
    if(!(localStorage.getItem("tema") == null)) {
        document.body.classList = localStorage.getItem("tema");
    }
    if(!(localStorage.getItem("textoTema") == null)) {
        document.getElementById("temVis").innerHTML = localStorage.getItem("textoTema");
    }
};

Carga_Tema();


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

/*----------------Funcionamiento de los cambios de estilo-------------------*/
/*------------------en los botones de control del sistema-------------------*/
/*--------------------------------de bombeo---------------------------------*/

/*var botonControl = document.querySelectorAll("a");
botonControl.forEach(enlace => {
    enlace.addEventListener("click", function(event) {
        let clickadas = 1; 
        console.log("Evento 'event': ");
        console.log(event.target);
        let solHref = event.target.href;
        console.log("href del botón: " + solHref);
    });
});*/

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
};

  // Funciones para actualizar el estado del sistema en la ventana /index.
  
    function Actualiza_Estado() {
        console.log("Entrando a Actualiza_Estado().");
        var actEstado = new XMLHttpRequest();
        let mSistema = ["MANUAL", "AUTO"];
        actEstado.onreadystatechange = function() {
            if(this.readyState == 4 && this.status == 200)
            {
                console.log("Analizando los componentes reibidos en Actualiza_Estado()");
                var estadoWeb = JSON.parse(this.responseText);
                console.log(estadoWeb);
                var estadoEnc = document.getElementById("estado-sistema");
                estadoEnc.innerHTML = "<p>Estado Sistema: " + "<strong>" + `${estadoWeb["estado-sistema"]}` + "</strong>";
                var modoEnc = document.getElementById("modo-sistema");
                if(estadoWeb["modo-sistema"])
                {
                    modoEnc.innerHTML = "<p>Modo Sistema: " + "<strong>" + `${mSistema[1]}` + "</strong>";
                }
                else if(!estadoWeb["modo-sistema"])
                {
                    modoEnc.innerHTML = "<p>Modo Sistema: " + "<strong>" + `${mSistema[0]}` + "</strong>";
                };
                console.log("Enviados los componentes a actualizar a la pantalla");
            }
            else
            {
                console.log("Contenido no cargado en Actualiza_Estado()");
                
            }
        };
        actEstado.open("HTTP_GET", "/data-estado", true);
        actEstado.send();
        console.log("Solicitada la actualización del estado del sistema");
    };

    // Funciones para mostrar el nivel del tanque en la página de visualización.

    function Lee_Nivel_Tanque() {
        var leeEstadoTanque = new XMLHttpRequest();
        leeEstadoTanque.onreadystatechange = function() {
            if(this.readyState == 4 && this.status == 200)
            {
                var volTanqueWeb = JSON.parse(this.responseText);
                console.log(volTanqueWeb);
                var volActualWeb = document.getElementById("mask01");
                volActualWeb.innerHTML = volTanqueWeb.colAgua.toFixed(2);
            }
            else
            {
                console.log("Contenido no cargado en Lee_Nivel_Tanque()");
            }
        };
        leeEstadoTanque.open("HTTP_GET", "/data-tanque", true);
        leeEstadoTanque.send();
        console.log("Solicitada actualización del tanque.");
    };

    // Funciones para enviar la hora y la fecha a la ventana

    function Fecha_Hora() {
        let hoyFecha = new Date();
        let opcionesHora = {hour: "2-digit", minute: "2-digit", second: "2-digit"};
        let hoyHora = hoyFecha.toLocaleTimeString([], opcionesHora);
        let dia = hoyFecha.getDate();
        let mes = hoyFecha.getMonth() + 1;
        let anio = hoyFecha.getFullYear();
        let diaSemana = hoyFecha.getDay();
        dia = ('0' + dia).slice(-2);
        mes = ('0' + mes).slice(-2);
        let semana = ['DOMINGO', 'LUNES', 'MARTES', 'MIÉRCOLES', 'JUEVES', 'VIERNES', 'SÁBADO'];
        let muestraSemana = semana[diaSemana];
        let fechaCompleta = `${muestraSemana}-${dia}-${mes}-${anio}`;
        document.getElementById("la-fecha").innerHTML = fechaCompleta;
        document.getElementById("la-hora").innerHTML = hoyHora;
        console.log(fechaCompleta + '->' + hoyHora);
        if(window.location.pathname == "/config")
        {
            document.getElementById("fecha").value = `${anio}-${mes}-${dia}`;
            document.getElementById("hora-actual").value = hoyHora;
        }
    };

    // Actualización de estado del sistema en la página /index.

    function Botones_Estado_Sistema()
    {
        const contenidoDiv = document.querySelector(".contenido");
        const botonControlElementos = contenidoDiv.querySelectorAll(".boton-control");
        botonControlElementos.forEach(elemento => {
            elemento.addEventListener("click", function(event) {
                const elementoPresionado = event.target.id;
                console.log(`Botón de control presionado: ${elementoPresionado}`);
                const dirBotonControl = ["/apagar", "/encender", "/auto", "/manual"];
                var solBotonControl = new XMLHttpRequest();
                switch(elementoPresionado)
                {
                    case "boton-control-1":
                    {
                        solBotonControl.open("HTTP_GET", `${dirBotonControl[0]}`, true);
                        console.log(`Botón presionado: ${dirBotonControl[0]}`);
                    }
                    break;
                    case "boton-control-2":
                    {
                        solBotonControl.open("HTTP_GET", `${dirBotonControl[1]}`, true);
                        console.log(`Botón presionado: ${dirBotonControl[1]}`);
                    }
                    break;
                    case "boton-control-3":
                    {
                        solBotonControl.open("HTTP_GET", `${dirBotonControl[2]}`, true);
                        console.log(`Botón presionado: ${dirBotonControl[2]}`);
                    }
                    break;
                    case "boton-control-4":
                    {
                        solBotonControl.open("HTTP_GET", `${dirBotonControl[3]}`, true);
                        console.log(`Botón presionado: ${dirBotonControl[3]}`);
                    }
                    break;
                    default:
                    {
                        console.warn("El botón presionado no está listado.");
                    }
                    break;
                };
                solBotonControl.send();
                Actualiza_Estado();
            });
        });
    };
    
    // Procesos a realizar una vez se ha cargado una página web.
    window.onload = function() {        
        let intervaloVolumen;
        let intervaloFecha;
        intervaloFecha = setInterval(Fecha_Hora, 1000);
        if((window.location.pathname === "/index"))
        {
                console.log("Se configura la lectura del nivel del tanque.");
                intervaloVolumen = setInterval(Lee_Nivel_Tanque, 20000);
                Botones_Estado_Sistema();
        }
        else
        {
            console.log("Se cancela el intervalo y se reinicia la bandera de carga de la página.");
            clearInterval(intervaloVolumen);
            intervaloVolumen = null;
        }
    };