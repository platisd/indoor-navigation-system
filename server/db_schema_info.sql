CREATE TABLE location
(
  device_id INTEGER PRIMARY KEY,
  employee_id TEXT,
  pos_x REAL,
  pos_y REAL,
  pos_z REAL,
  timestamp datatime default current_timestamp
);


CREATE TABLE sample_dev_10
(
  id INTEGER PRIMARY KEY,
  mac_addr TEXT,
  rssi REAL,
  timestamp datatime default current_timestamp
);

CREATE TABLE sample_dev_11
(
  id INTEGER PRIMARY KEY,
  mac_addr TEXT,
  rssi REAL,
  timestamp datatime default current_timestamp
);


CREATE TABLE sample_dev_1034
(
  id INTEGER PRIMARY KEY,
  mac_addr TEXT,
  rssi REAL,
  timestamp datatime default current_timestamp
);


CREATE TABLE sample_dev_103
(
  id INTEGER PRIMARY KEY,
  mac_addr TEXT,
  rssi REAL,
  timestamp datatime default current_timestamp
);
