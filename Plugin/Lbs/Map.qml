import QtQuick 2.0
import QtLocation 5.3

Rectangle {
    width: 100
    height: 100
    
    Plugin {
        id: mapPlugin
        // code here to choose the plugin as necessary
    }
    
    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
    
        center {
            latitude: -27
            longitude: 153
        }
        zoomLevel: map.minimumZoomLevel

        gesture.enabled: true
    }
}

