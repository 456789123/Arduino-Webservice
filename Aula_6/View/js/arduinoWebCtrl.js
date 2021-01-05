angular.module("arduinoWeb").controller("arduinoWebCtrl", function ($scope, arduinoWebService, $interval ) {

    $scope.texto = new String( );
    $scope.umidadeTemperatura = {temperatura: 15, umidade: 50};

    $scope.enviarTexto = function( ) {
        let objeto = new Object( );
        objeto.texto = $scope.texto;
        arduinoWebService.enviarTexto( objeto ).then( function( success ){
            $scope.texto = new String( );
        });
    }

    $interval( function( ) {
		arduinoWebService.getTemperaturaUmidade( ).then( function(success){
            $scope.umidadeTemperatura.temperatura = success.data.temperatura;
            $scope.umidadeTemperatura.umidade = success.data.umidade;
        }, function(error) {
		    console.log("Aconteceu um problema!!");
		});
    }, 500);

});