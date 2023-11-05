let value = document.querySelector('#value');
let loader = document.querySelector('#loader')
// let btn_regar =  document.querySelector('#btn-regar');
let btn_atualizar = document.querySelector('#btn-atualizar');
// let btn_txt_regar =  document.querySelector('#btn-txt-regar');
let btn_txt_atualizar =  document.querySelector('#btn-txt-atualizar');

document.addEventListener("DOMContentLoaded", function () {
    trazerDados();
    setInterval(function() {
        trazerDados();
    }, 60000) //dados sao atualizados a cada 60 segundos
});

function alternarItens(){
    value.classList.toggle("hide");
    loader.classList.toggle("hide");
}

async function trazerDados(){
    console.log('trazer dados')
    alternarItens();
    // btn_regar.disabled = true;
    btn_atualizar.disabled = true;
    fetch('http://192.168.0.128/esp8266/dados', {    method: "GET",   })
        .then((response) => {   return response.json() })
        .then((data) => {   value.innerHTML = JSON.stringify(data.dados.valor_pct) + "%";  })
        .finally(() => {
            alternarItens();
            btn_txt_atualizar.innerHTML = 'Atualizar dados'
            // btn_regar.disabled = false;
            btn_atualizar.disabled = false;
        })
        .catch((error) => {
            console.log(error);
            alternarItens();
            btn_txt_atualizar.innerHTML = 'Atualizar dados';
            value.innerHTML = "erro";
            // btn_regar.disabled = false;
            btn_atualizar.disabled = false;
        });
}

// async function regar(){
//     console.log('regar planta')
//     btn_txt_regar.innerHTML = 'Regando...'
//     fetch('http://192.168.0.128/esp8266/regar', {   method: 'GET', mode: 'no-cors'   })
//     .then((response) => console.log(response))
//     .finally(() => btn_txt_regar.innerHTML = 'Clique para regar')
// }