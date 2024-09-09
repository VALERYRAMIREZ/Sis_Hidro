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

 /* Variable para controlar el encendido de la bomba de agua en modo manual */

 if(document.getElementById("tile01") != null)
{
    var bombaManual = document.getElementById("tile01");
    bombaManual.addEventListener("click", function() {
    var encBombaManual = new XMLHttpRequest();
    encBombaManual.onreadystatechange = function() {
        if(this.readyState == 4 && this.status == 200)
        {
            console.log("Analizando si la bomba ha sido encendida.");
            var estadoBomba = JSON.parse(this.responseText);
            console.table(estadoBomba);
            if(estadoBomba["Bomba"] == "1")
            {
                bombaManual.style.borderWidth = "5px";
                bombaManual.style.borderStyle = "solid";
                bombaManual.style.borderColor = "white";
                bombaManual.style.transition = "all 0.4s";
            }
            else if(estadoBomba["Bomba"] === "0")
            {
                bombaManual.style.borderWidth = "1px";
                bombaManual.style.borderStyle = "solid";
                bombaManual.style.borderColor = "white";
                bombaManual.style.transition = "all 0.4s";
            }
            else
            {
                console.warn("El estilo a aplicar al indicador de estado de bomba no existe.");
            }
        }
    }
    encBombaManual.open("GET", "/enc-manual", true);
    encBombaManual.send();
});
}
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

if(document.getElementById("forma-tipo-tanque") != null) {
    var elementoTipoForma = document.getElementById("forma-tipo-tanque");
    elementoTipoForma.addEventListener("submit", function(e) {
        e.preventDefault();
        var datosTipoForma = {};
        var formaDatosTipo = new FormData(elementoTipoForma);
        for(var [k,v] of formaDatosTipo) {
            datosTipoForma[k] = v;
        }
        console.log(datosTipoForma);
        var datosJsonTipo = JSON.stringify(datosTipoForma)
        console.log(datosJsonTipo);
        var configTipo = new Blob([datosJsonTipo], {type: 'application/json'});
        fetch("forma-tipo-datos", {
            method: "POST",
            headers: {
                'Content-Type': 'application/json',
            },
            body: datosJsonTipo 
        }).then(response => {
            if(!response.ok) {
                throw new Error("Respuesta de la red invalida");
            }
        }).then(data => {
            console.log("Datos recibidos del servior", data);
            alert("Tipp de tanque configurado");
        }).catch(function maneja_error(error) {
            console.error("Error: ", error);
            alert("Ocurrio un error y no se configuro el tipo de tanque");
        });
        console.log(configTipo);
    });

};

if(document.getElementById("forma-intervalos-tanque") != null) {
    var elementoIntervaloForma = document.getElementById("forma-intervalos-tanque");
    elementoIntervaloForma.addEventListener("submit", function(e) {
        e.preventDefault();
        var datosIntervalosForma = {};
        var formaDatosIntervalos = new FormData(elementoIntervaloForma);
        for(var [k,v] of formaDatosIntervalos) {
            datosIntervalosForma[k] = v;
        }
        console.log(datosIntervalosForma);
        var datosJsonIntervalos = JSON.stringify(datosIntervalosForma)
        console.log(datosJsonIntervalos);
        var configIntervalos = new Blob([datosJsonIntervalos], {type: 'application/json'});
        fetch("forma-intermed-tanque", {
            method: "POST",
            headers: {
                'Content-Type': 'application/json',
            },
            body: datosJsonIntervalos 
        }).then(response => {
            if(!response.ok) {
                throw new Error("Respuesta de la red invalida");
            }
            return response.responseText;
        }).then(data => {
            console.log("Datos recibidos del servdor: ", data);
            alert("Intervalo de medicion configurado");
        }).catch(function maneja_error(error) {
            console.error("Error: ", error);
            alert("Ocurrio un error y no se configuro el intervalo de medicion");
        });
        console.log(configIntervalos);
    });
};

if(document.getElementById("forma-numero-tanque") != null) {
    var tanqueNumeroForma = document.getElementById("forma-numero-tanque");
    tanqueNumeroForma.addEventListener("submit", function(e) {
        e.preventDefault();
        var datosTanqueNumeroForma = {};
        var formaTanqueNumeroTipo = new FormData(tanqueNumeroForma);
        for(var [k,v] of formaTanqueNumeroTipo) {
            datosTanqueNumeroForma[k] = v;
        }
        console.log(datosTanqueNumeroForma);
        var datosJsonNumeroTanque = JSON.stringify(datosTanqueNumeroForma)
        console.log(datosJsonNumeroTanque);
        var configTipo = new Blob([datosJsonNumeroTanque], {type: 'application/json'});
        fetch("forma-numero-bomba", {
            method: "POST",
            headers: {
                'Content-Type': 'application/json',
            },
            body: datosJsonNumeroTanque
        }).then(response => {
            if(!response.ok) {
                throw new Error("Respuesta de la red invalida");
            }
        }).then(data => {
            console.log("Datos recibidos del servior", data);
            alert("Número de bombas configurado");
        }).catch(function maneja_error(error) {
            console.error("Error: ", error);
            alert("Ocurrio un error y no se configuro el tipo de tanque");
        });
        console.log(configTipo);
    });
};

if(document.getElementById("forma-tiempos-tanque") != null) {              // Si encuentra la forma con la id igual a "forma",
    var elementoTiempoForma = document.getElementById("forma-tiempos-tanque");   // extrae los valores completos.
    elementoTiempoForma.addEventListener("submit", function(e) {
        e.preventDefault();
        var datosTiemposForma = {};
        var formaDatosTiempos = new FormData(elementoTiempoForma);
        for(var [k,v] of formaDatosTiempos) {
            datosTiemposForma[k] = v;
        }
        console.log(datosTiemposForma);
        var datosJsonTiempos = JSON.stringify(datosTiemposForma)
        console.log(datosJsonTiempos);
        var configTiempos = new Blob([datosJsonTiempos], {type: 'application/json'});
        fetch("forma-tiempos-datos", {
            method: "POST",
            headers: {
                'Content-Type': 'application/json',
            },
            body: datosJsonTiempos
        }).then(response => {
            if(!response.ok) {
                throw new Error("Respuesta de la red invalida");
            }
        }).then(data => {
            console.log("Datos recibidos del servior", data);
            alert("Intervalos de funcionamiento configurados");
        }).catch(function maneja_error(error) {
            console.error("Error: ", error);
            alert("Ocurrio un error y no se configurraron los intervalos de funcionamiento");
        });

        /*fs.writeFile('config.json', datosJsonTiempos, 'utf-8', (err) => {
            if(err) {
                throw err;
            }
            console.log("Archivo guardado correctamente");
        });*/
        console.log(configTiempos);
        alert("Tiempos de funcionamiento del sistema configurados")
        /*var envioForma = new XMLHttpRequest();
        envioForma.open("POST", "forma-tiempos-datos", true);
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
                alert("Enviados los componentes a actualizar a la pantalla");
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
                volActualWeb
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
        //console.log(fechaCompleta + '->' + hoyHora);
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