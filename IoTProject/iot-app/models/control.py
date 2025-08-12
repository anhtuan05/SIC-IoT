from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
from extensions import db

class Control(db.Model):
    __tablename__ = 'controls'
    id = db.Column(db.Integer, primary_key=True)
    device = db.Column(db.String(50), nullable=False)  # 'light' or 'fan' 'pump'
    state = db.Column(db.Boolean, nullable=False)      # True (on) or False (off)
    timestamp = db.Column(db.DateTime, default=datetime.utcnow)
