from flask import Blueprint, request, jsonify
from models.sensor import Sensor, db
from models.control import Control
from flask_login import login_required

bp = Blueprint('api', __name__)

@bp.route('/api/sensor', methods=['POST'])
def receive_sensor_data():
    pass

@bp.route('/api/control', methods=['POST'])
@login_required
def control_device():
    pass

@bp.route('/api/control/status', methods=['GET'])
def get_control_status():
    pass