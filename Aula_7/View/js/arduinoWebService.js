angular.module("arduinoWeb").factory("arduinoWebService", function( $http ) {
    const baseUrl = 'http://192.168.0.12:3000'
    const _getTemperaturaUmidade = function( ) {
		return $http.get( baseUrl + '/dadosTempo');
    }

    const _enviarTexto = function( objeto ) {
        return $http.post( baseUrl + '/enviarMensagen', objeto );
    }

    const _statusLed = function( objeto ) {
        return $http.post( baseUrl + '/statusLed', objeto );
    }
    
    return {
        getTemperaturaUmidade: _getTemperaturaUmidade,
        enviarTexto: _enviarTexto,
        statusLed: _statusLed
    }
});