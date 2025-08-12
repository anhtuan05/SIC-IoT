from flask import Blueprint, render_template, request, redirect, url_for
from flask_login import login_required, current_user
from models.user import User, db
from models.sensor import Sensor, db
from models.control import Control, db


bp = Blueprint('dashboard', __name__)

@bp.route('/dashboard')
@login_required
def index():
    sensors = Sensor.query.order_by(Sensor.timestamp.desc()).limit(10).all()
    sensors_data = [
        {
            'temperature': sensor.temperature,
            'humidity_air': sensor.humidity_air,
            'humidity_soil': sensor.humidity_soil,
            'timestamp': sensor.timestamp.strftime('%Y-%m-%d %H:%M:%S')
        } for sensor in sensors
    ]

    devices = ['fan', 'light', 'pump']
    controls_data = []
    for device in devices:
        latest_control = Control.query.filter_by(device=device).order_by(Control.timestamp.desc()).first()
        controls_data.append({
            'device': device.capitalize(),
            'status': latest_control.state if latest_control else False,
            'timestamp': latest_control.timestamp.strftime('%Y-%m-%d %H:%M:%S') if latest_control else None
        })

    avg_data = {
        'avg_temp': 0.0,
        'avg_humidity_air': 0.0,
        'avg_humidity_soil': 0.0
    }
    if sensors:
        avg_temp = db.session.query(db.func.avg(Sensor.temperature)).scalar() or 0.0
        avg_humidity_air = db.session.query(db.func.avg(Sensor.humidity_air)).scalar() or 0.0
        avg_humidity_soil = db.session.query(db.func.avg(Sensor.humidity_soil)).scalar() or 0.0
        avg_data = {
            'avg_temp': round(float(avg_temp), 2),
            'avg_humidity_air': round(float(avg_humidity_air), 2),
            'avg_humidity_soil': round(float(avg_humidity_soil), 2)
        }

    return render_template('dashboard.html', sensors=sensors_data, controls=controls_data, avg_data=avg_data)


@bp.route('/users', methods=['GET', 'POST'])
@login_required
def manage_users():
    if not current_user.is_admin:
        return redirect(url_for('dashboard.index'))
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        is_admin = 'is_admin' in request.form
        user = User(username=username, is_admin=is_admin)
        user.set_password(password)
        db.session.add(user)
        db.session.commit()
        return redirect(url_for('dashboard.manage_users'))
    users = User.query.all()
    return render_template('user_management.html', users=users)