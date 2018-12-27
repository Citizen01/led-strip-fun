(function () {
    // Create a client instance
    var client = new Paho.MQTT.Client("broker.hivemq.com", 8000, "myclientid_" + parseInt(Math.random() * 100, 10));

    // set callback handlers
    client.onConnectionLost = function (responseObject) {
        if (responseObject.errorCode !== 0) {
            console.log("onConnectionLost:" + responseObject.errorMessage);
        }
    };

    client.onMessageArrived = function (message) {
        console.log("onMessageArrived:" + message.payloadString);
    };

    window.client = client
})();

(function () {
    $("#btn-con").click(function() {
        var self = this;
        $(self).text("Connecting ...");
        client.connect({
            onSuccess: function () {
                $("#btn-con").hide();
                $("#btn-con").text("Connect");
                $("#btn-disco").show();

                console.log("onConnect");
                client.subscribe("ledstripfun");
                message = new Paho.MQTT.Message("Hello");
                message.destinationName = "ledstripfun";
                client.send(message);
            },
            onFailure: function (message) {
                $(self).text("Connect");
                console.log("Connection failed: " + message.errorMessage);
            }
        });
    });

    $("#btn-disco").click(function() {
        $(this).text("Disconnecting ...");
        client.disconnect();
        $("#btn-con").show();
        $("#btn-disco").hide();
        $("#btn-disco").text("Disconnect");
    });

})();

function send(beta) {
    if (client.isConnected()) {   
        message = new Paho.MQTT.Message(""+beta);
        message.destinationName = "ledstripfun";
        client.send(message);
    }
}

function getCurrentTimestamp() { return +new Date(); }

var updateTimerMs = 100;
var lastUpdateTimestamp = getCurrentTimestamp();
var lastBetaValue = 0;
(function () {
    var gn = new GyroNorm();
    gn.init().then(function () {
        gn.start(function (data) {
            $("#accel").text(JSON.stringify(data.do));
            var beta = data.do.beta;
            var betaDeviance = Math.abs(beta - lastBetaValue)
            if (getCurrentTimestamp() - lastUpdateTimestamp >= 500 || betaDeviance > 0.5) {
                send(beta);
                lastBetaValue = data.do.beta;
                lastUpdateTimestamp = getCurrentTimestamp();
            }
        });
    }).catch(function (e) {
        // Catch if the DeviceOrientation or DeviceMotion is not supported by the browser or device
        console.error(e);
    });
})();