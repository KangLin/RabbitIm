import QtQuick 2.5
import QtQuick.Controls 1.4
import QtLocation 5.5
import QtPositioning 5.5
import "../js/helper.js" as Helper

Rectangle {
    width: 102
    height: 100
    border.width: 1
        
    Plugin {
        id: mapPlugin
        //name: "google"
        preferred:["google", "osm"]
        PluginParameter { name: "google.mapping.host"; value: "www.google.cn" }
        //PluginParameter { name: "google.mapping.locale"; value: "cn" }
    }

    Map {
        id: map
        objectName: "objNameMap"
        
        property variant marker:Qt.createQmlObject ('Marker {}', map)
        property variant scaleLengths: [5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000, 1000000, 2000000]
        
        function init() {
            map.removeMapItem(map.marker);
            mapPolyline.path = []
        }
        
        function calculateScale() {
            var coord1, coord2, dist, text, f
            f = 0
            coord1 = map.toCoordinate(Qt.point(0,scale.y))
            coord2 = map.toCoordinate(Qt.point(0+scaleImage.sourceSize.width,scale.y))
            dist = Math.round(coord1.distanceTo(coord2))
    
            if (dist === 0) {
                // not visible
            } else {
                for (var i = 0; i < scaleLengths.length-1; i++) {
                    if (dist < (scaleLengths[i] + scaleLengths[i+1]) / 2 ) {
                        f = scaleLengths[i] / dist
                        dist = scaleLengths[i]
                        break;
                    }
                }
                if (f === 0) {
                    f = dist / scaleLengths[i]
                    dist = scaleLengths[i]
                }
            }
    
            text = Helper.formatDistance(dist)
            scaleImage.width = (scaleImage.sourceSize.width * f) - 2 * scaleImageLeft.sourceSize.width
            scaleText.text = text
        }
        
        function moveMarker(coor) {
            map.removeMapItem(map.marker);
            map.marker.coordinate = coor
            map.addMapItem(map.marker)    
        }
        
        function addPolylinePoint(coor) {
            mapPolyline.addCoordinate(coor);
            map.center = coor
            moveMarker(coor)
        }

        anchors.fill: parent
        plugin: mapPlugin
        center {
            latitude: 27.745549
            longitude: 111.296072
        }
        gesture.enabled: true
        onZoomLevelChanged:{
            if(zoomSlider.value != zoomLevel) {
                zoomSlider.value = zoomLevel
            }
            map.calculateScale()
        }
        Keys.onPressed: {
            if (event.key === Qt.Key_Plus) {
                map.zoomLevel++
            } else if (event.key === Qt.Key_Minus) {
                map.zoomLevel--
            }
        }
        Component.onCompleted: {
            for(var i = 0; i < map.supportedMapTypes.length; i++) {
                if(MapType.HybridMap === map.supportedMapTypes[i]) {
                    map.activeMapType = map.supportedMapTypes[i]
                    map.mapPolyline.line.color = "red"
                }
            }
            moveMarker(map.center)
        }

        MouseArea{
            anchors.fill: parent
            onClicked: map.zoomLevel = map.zoomLevel << 1
        }
        MultiPointTouchArea{
            anchors.fill: parent
            //gestureStarted:
        }

        MapPolyline {
            id: mapPolyline
            line.color: "green"
            line.width: 4
            opacity: 0.5
            smooth: true            
        }

        Slider {
            id: zoomSlider;
            z: map.z + 3
            minimumValue: map.minimumZoomLevel;
            maximumValue: map.maximumZoomLevel;
            anchors.margins: 10
            anchors.bottom: scale.top
            anchors.top: parent.top
            anchors.right: parent.right
            orientation : Qt.Vertical
            value: map.maximumZoomLevel - ((map.maximumZoomLevel - map.minimumZoomLevel) >> 2)
            onValueChanged: {
                map.zoomLevel = value;
            }
        }

        Item {
            id: scale
            z: map.z + 3
            visible: scaleText.text != "0 m"
            anchors.bottom: parent.bottom;
            anchors.right: parent.right
            anchors.margins: 20
            height: scaleText.height * 2
            width: scaleImage.width

            Image {
                id: scaleImageLeft
                source: "../png/scale_end.png"
                anchors.bottom: parent.bottom
                anchors.right: scaleImage.left
            }
            Image {
                id: scaleImage
                source: "../png/scale.png"
                anchors.bottom: parent.bottom
                anchors.right: scaleImageRight.left
            }
            Image {
                id: scaleImageRight
                source: "../png/scale_end.png"
                anchors.bottom: parent.bottom
                anchors.right: parent.right
            }
            Label {
                id: scaleText
                color: "#004EAE"
                anchors.centerIn: parent
                text: "0 m"
            }
            Component.onCompleted: {
                map.calculateScale();
            }
        }

    }
}

