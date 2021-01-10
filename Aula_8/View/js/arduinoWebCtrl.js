angular.module("arduinoWeb").controller("arduinoWebCtrl", function ($scope, arduinoWebService, $interval ) {

    $scope.texto = new String( );
    $scope.umidadeTemperatura = {temperatura: 15, umidade: 50};
    $scope.led = { led1: 0, led2: 0, led3: 0, led4: 0 };
    $scope.pwm = 0;
    $scope.percente = 0;

    $scope.enviarTexto = function( ) {
        let objeto = new Object( );
        objeto.texto = $scope.texto;
        arduinoWebService.enviarTexto( objeto ).then( function( success ){
            $scope.texto = new String( );
        });
    }

    $scope.ledOnOff = function( ) {
        arduinoWebService.statusLed($scope.led);
    }

    $scope.funcaoPWM = function( ) {
        $scope.percente = ($scope.pwm*100)/255;
        $scope.percente = parseInt($scope.percente);
        arduinoWebService.pwmLed(valorPwmString( ));
    }
    
    $interval( function( ) {
		arduinoWebService.getTemperaturaUmidade( ).then( function(success){
            $scope.umidadeTemperatura.temperatura = success.data.temperatura;
            $scope.umidadeTemperatura.umidade = success.data.umidade;
        }, function(error) {
		    console.log("Aconteceu um problema!!");
		});
    }, 500);

    valorPwmString = function( ) {
        let strNumero = '';
        if( $scope.pwm < 10 )  strNumero = '00' + $scope.pwm.toString( ); else
        if( $scope.pwm < 100 ) strNumero =  '0' + $scope.pwm.toString( ); else
        strNumero = $scope.pwm.toString( );
        let objeto = { pwm: strNumero };
        return objeto;
    }

});