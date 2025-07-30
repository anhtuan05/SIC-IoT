from datetime import timedelta

from flask import Flask
from extensions import db, login_manager
from config import Config

app = Flask(__name__)
app.config.from_object(Config)

db.init_app(app)
login_manager.init_app(app)
login_manager.login_view = 'auth.login'
app.config['PERMANENT_SESSION_LIFETIME'] = timedelta(minutes=10)

from models.user import User
from models.control import Control
from models.sensor import Sensor
from routes import auth, api, dashboard

app.register_blueprint(auth.bp)
app.register_blueprint(api.bp)
app.register_blueprint(dashboard.bp)

@login_manager.user_loader
def load_user(user_id):
    return User.query.get(int(user_id))

with app.app_context():
    db.create_all()

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')