import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0
import ".."

Page{
    id: video_test
    height: parent.height; width: parent.width

    Camera{
        id: camera
        imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash
        imageCapture{
            onImageCaptured: {
                photoPreview.source = preview
                console.log(preview)
            }
        }
        onError: function(code_err, msg_err) {
            console.log(code_err)
            console.log(msg_err)
        }
    }
    Column{
        VideoOutput{
            source: camera
            anchors.fill: parent
            focus: visible
        }
        Image{
            id: photoPreview
        }
    }
}
