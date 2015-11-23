TEMPLATE = subdirs

lbs_app.SUBDIRS = Lbs
lbs_app.file = Lbs/Lbs_App.pro

QRCode_App.SUBDIRS = QRCode
QRCode_App.file = QRCode/QRCode_App.pro

SUBDIRS += QRCode Lbs 

!android {
    SUBDIRS += QRCode_App lbs_app
}
