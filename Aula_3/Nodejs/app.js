const SerialPort = require('serialport');
const express    = require('express');

//Variaveis globais...
var str = "";
var objeto = {'umidade':'', 'temperatura':''};

const app  = express( );
const port = new SerialPort('/dev/ttyACM0', {
    baudRate: 9600
});

// Enviar para o Arduino...
var enviar = function( mensagem ) {
    port.write( mensagem, function(err) {
       if (err) {
           return console.log('Erro ao enviar: ', err.message);
       }
       console.log('Mensagem enviada!!!');
   });
}

// Recebe do Arduino e transforma em objeto Json...
port.on( 'data', function( data ) {
    //console.log(data.toString( ));
    str += data.toString( );
    var subStr = str.split('-');
    
    for( var i = 0; i < subStr.length; i++ ) {
        if( 'humidit' === subStr[i] ) {
	    	if( undefined !== subStr[i+1] )
            	objeto.umidade = subStr[ i + 1 ];
		} else if( 'temper' === subStr[i] ) {
			if( undefined !== subStr[i+1] )
			    objeto.temperatura = subStr[ i + 1 ];
		}
    }

    if( 4 > subStr.length ) {
        str    = new String( );
	    subStr = new String( );
    }

    console.log(objeto);
});

// Teste do Webservice
app.get('/', function(req, res) {
    res.send('Arduino Webservice');
});

// Envia do Webservice para o Arduino
app.get('/mensagem', function (req, res) {
    enviar('  Arduino Web     PODE CRE!!\\');    
    res.send('Mensagem enviada');
});

// Recebe os padrões de temperatura e umnidade através do objeto
app.get('/dadosTempo', function (req, res) {
    res.status(200).send(objeto);
})

//Porta do Webservice para o Arduino
app.listen(3000);
