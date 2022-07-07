import 'package:flutter_dotenv/flutter_dotenv.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';

import 'package:on_sight/backend/backend_database.dart';
import 'package:on_sight/localisation/localisation_localisation.dart';
import 'package:on_sight/navigations/navigations_navigations.dart';
import 'package:on_sight/mqtt/mqtt_mqtt.dart';

class OnSight {
  // ==== Private Methods ====
  OnSight();

  late MyDatabase _db;
  late Localisation _lc;
  late Mqtt _mq;
  late MyShortestPath _sp;

  // ==== Public Methods ====
  /// Runs the localisation algorithm
  ///
  /// Inputs:
  /// 1) None.
  ///
  /// Returns:
  /// 1) None.
  Future start() async {
    // DotEnv
    await dotenv.load(fileName: './lib/assets/.env');

    // DynamoDB
    _db = MyDatabase(
      region: dotenv.env['awsRegion'].toString(),
      endPointUrl: dotenv.env['awsEndPoint'].toString(),
    );
    await _db.init();

    // MQTT
    _mq = Mqtt(
      host: dotenv.env['mqttHost'].toString(),
      username: dotenv.env['mqttUsername'].toString(),
      password: dotenv.env['mqttPassword'].toString(),
    );
    await _mq.init();

    // Localisation
    _lc = Localisation(dbObj: _db);

    // Shortest Path
    _sp = MyShortestPath(dbObj: _db);

    _testShortestPath([400.0, 0.0], [1500.0, 1200.0]);
  }

  /// TODO: TO delete in production code
  /// To test if shortest path algorithm runs properly
  void _testShortestPath(List<double> start, List<double> goal) {
    _sp.setup(start, goal);
    print(_sp.determineShortestPath());
  }

  /// Wrapper function for localisation.
  ///
  /// Inputs:
  /// 1) rawData [Map<String, dynamic>] -
  /// e.g. {
  ///         'rssi': {
  ///           'd94250a2-c73a-4249-9a1e-4abb2643078a': -74.35,
  ///           '87ccf436-0f86-4dfe-80f9-9ff731033620': -65.25,
  ///           '9d9214f8-8870-43dd-a496-401765bf7866': -65.75
  ///         },
  ///         'accelerometer': 5,
  ///         'magnetometer': [-33.57, 86.31]
  ///      }
  ///
  /// Returns:
  /// 1) estimated position [<Map<String,dynamic>>]
  Map<String, dynamic> localisation(Map<String, dynamic> rawData) {
    return _lc.localisation(rawData);
  }

  /// Wrapper function for publishing data points to mqtt server.
  ///
  /// Inputs:
  /// 1) rawData [Map<String, dynamic>] -
  /// e.g. {
  ///         'x_coordinate': X,
  ///         'y_coordinate': Y,
  ///         'direction':direction
  ///      }
  /// 2) mode [String] - either 'rssi' or 'result'.
  /// 2) topic [String] - default is 'test/pub'.
  ///
  /// Returns:
  /// 1) None.
  void mqttPublish(Map<String, dynamic> rawData, String mode,
      {String topic = 'test/pub'}) {
    _mq.publish(rawData, mode, topic: topic);
  }

  /// Function to retrieve known uuid from database.
  ///
  /// Inputs:
  /// None.
  ///
  /// Return:
  /// knownUuid [List<Uuid>].
  List<Uuid> getKnownUuid() {
    return _db.getKnownUuid();
  }

  /// Function to retrieve known MAC address from database.
  ///
  /// Inputs:
  /// None.
  ///
  /// Return:
  /// knownUuid [List<String>].
  List<String> getKnownMac() {
    return _db.getKnownMac();
  }
}
