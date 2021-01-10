const SerialPort = require('serialport');
const express    = require('express');
const bodyParser = require("body-parser");
const cors = require('cors');

//Variaveis globais...
var str = new String( );
var objetoTempo = {'umidade':'', 'temperatura':''};
var objetoEnviar = new String( );
var statusLed = "0000";
var texto = new String( );

const app  = express( );
app.use(cors( ));
const port = new SerialPort('/dev/ttyACM0', {
    baudRate: 9600
});

app.use(bodyParser.json( ));
app.use(bodyParser.urlencoded({ extended: true }));

// Conecta a parte da pagina Web para a aplcação...
app.use(express.static('../View'));

/// Enviar para o Arduino...
var enviar = function( mensagem ) {
    port.write( mensagem, function(err) {
       if (err) {
           return console.log('Erro ao enviar: ', err.message);
       }
       //console.log('Mensagem enviada!!!');
   });
}

// Recebe do Arduino e transforma em objeto Json...
port.on( 'data', function( data ) {
    //console.log(data.toString( ));
    str += data.toString( );
    let subStr = str.split('-');
    
    for( var i = 0; i < subStr.length; i++ ) {
        if( 'humidit' === subStr[i] ) {
	    	if( undefined !== subStr[i+1] )
                objetoTempo.umidade = subStr[ i + 1 ];
		} else if( 'temper' === subStr[i] ) {
			if( undefined !== subStr[i+1] )
                objetoTempo.temperatura = subStr[ i + 1 ];
		}
    }

    if( 4 > subStr.length ) str = new String( );

    //console.log(objetoTempo);
});

// Envia do Webservice para o Arduino
app.post('/enviarMensagen', function (req, res) {
    texto = req.body.texto;
    objetoEnviar = '01' + texto + '\\';
    enviar(objetoEnviar);
    res.status(200).send( );
});

// Envia estatus dos LEDs para o Arduino
app.post('/statusLed', function (req, res) {
    let led1 = req.body.led1;
    let led2 = req.body.led2;
    let led3 = req.body.led3;
    let led4 = req.body.led4;

    statusLed = `${led1}${led2}${led3}${led4}`;
    objetoEnviar = '02' + statusLed + '\\';
    enviar(objetoEnviar);
    res.status(200).send( );
});

// Envia valor de PWM para o Arduino
app.post('/pwm', function (req, res) {
    let pwm = req.body.pwm;

    objetoEnviar = '03' + pwm + '\\';
    enviar(objetoEnviar);
    res.status(200).send( );
});

// Recebe os padrões de temperatura e umnidade através do objeto
app.get('/dadosTempo', function (req, res) {
    res.status(200).send(objetoTempo);
})

//Porta do Webservice para o Arduino
app.listen(3000);