from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
from extensions import db

class Sensor(db.Model):
    __tablename__ = 'sensors'
    id = db.Column(db.Integer, primary_key=True)
    temperature = db.Column(db.Float, nullable=False)
    humidity_air = db.Column(db.Float, nullable=False)
    humidity_soil = db.Column(db.Float, nullable=False)
    timestamp = db.Column(db.DateTime, default=datetime.utcnow)