from datetime import datetime

from flask import Blueprint, request, jsonify
from models.sensor import Sensor, db
from models.control import Control
from flask_login import login_required

bp = Blueprint('api', __name__)


@bp.route('/api/sensor', methods=['POST'])
def receive_sensor_data():
    try:
        data = request.get_json()
        if not data:
            return jsonify({'error': 'No JSON data provided'}), 400

        temperature = data.get('temperature')
        humidity_air = data.get('humidity_air')
        humidity_soil = data.get('humidity_soil')

        if None in (temperature, humidity_air, humidity_soil):
            return jsonify({'error': 'Missing required fields'}), 400

        # Validate data types and ranges
        try:
            temperature = float(temperature)
            humidity_air = float(humidity_air)
            humidity_soil = float(humidity_soil)
        except (ValueError, TypeError):
            return jsonify({'error': 'Invalid data format'}), 400

        if not (0 <= humidity_air <= 100 and 0 <= humidity_soil <= 100):
            return jsonify({'error': 'Humidity values out of range (0-100)'}), 400

        # Save sensor data to database
        sensor_data = Sensor(
            temperature=temperature,
            humidity_air=humidity_air,
            humidity_soil=humidity_soil,
            timestamp=datetime.utcnow()
        )
        db.session.add(sensor_data)
        db.session.commit()

        return jsonify({
            'message': 'Sensor data received successfully',
            'data': {
                'temperature': temperature,
                'humidity_air': humidity_air,
                'humidity_soil': humidity_soil,
                'timestamp': sensor_data.timestamp.isoformat()
            }
        }), 201

    except Exception as e:
        db.session.rollback()
        return jsonify({'error': str(e)}), 500


@bp.route('/api/control', methods=['POST'])
def control_device():
    try:
        data = request.get_json()
        if not data:
            return jsonify({'error': 'No JSON data provided'}), 400

        device = data.get('device')
        state = data.get('state')
        mode = data.get('mode', 'manual')  # Default to manual mode

        if device not in ['light', 'fan', 'pump']:
            return jsonify({'error': 'Invalid device name'}), 400

        if state not in [True, False, 'true', 'false']:
            return jsonify({'error': 'Invalid state value'}), 400

        # Convert string state to boolean if necessary
        state = True if state in [True, 'true'] else False

        # Save control command to database
        control = Control(
            device=device,
            state=state,
            timestamp=datetime.utcnow()
        )
        db.session.add(control)
        db.session.commit()

        return jsonify({
            'message': f'{device.capitalize()} turned {"on" if state else "off"} successfully',
            'data': {
                'device': device,
                'state': state,
                'mode': mode,
                'timestamp': control.timestamp.isoformat()
            }
        }), 200

    except Exception as e:
        db.session.rollback()
        return jsonify({'error': str(e)}), 500


@bp.route('/api/control/status', methods=['GET'])
def get_control_status():
    try:
        # Get the latest status for each device
        devices = ['light', 'fan', 'pump']
        status = {}

        for device in devices:
            latest_control = Control.query.filter_by(device=device).order_by(Control.timestamp.desc()).first()
            status[device] = {
                'state': latest_control.state if latest_control else False,
                'timestamp': latest_control.timestamp.isoformat() if latest_control else None
            }

        return jsonify({
            'message': 'Device status retrieved successfully',
            'data': status
        }), 200

    except Exception as e:
        return jsonify({'error': str(e)}), 500
