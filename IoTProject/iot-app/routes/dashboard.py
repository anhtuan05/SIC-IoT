from flask import Blueprint, render_template, request, redirect, url_for
from flask_login import login_required, current_user
from models.user import User, db


bp = Blueprint('dashboard', __name__)

@bp.route('/dashboard')
@login_required
def index():
    # Dữ liệu mẫu
    sensors = [
        {'temperature': 25.5, 'humidity_air': 60, 'humidity_soil': 45, 'timestamp': '2025-07-29 20:00'},
        {'temperature': 26.0, 'humidity_air': 62, 'humidity_soil': 47, 'timestamp': '2025-07-29 20:10'},
    ]
    controls = [
        {'device': 'Fan', 'status': True, 'timestamp': '2025-07-29 20:00'},
        {'device': 'Light', 'status': False, 'timestamp': '2025-07-29 20:05'},
        {'device': 'Pump', 'status': True, 'timestamp': '2025-07-29 20:05'},
    ]
    avg_data = {
        'avg_temp': 25.75,
        'avg_humidity_air': 61.0,
        'avg_humidity_soil': 46.0
    }
    return render_template('dashboard.html', sensors=sensors, controls=controls, avg_data=avg_data)


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