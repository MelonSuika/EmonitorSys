import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.5

Item {


    id: item1
    width: quickWidth
    height: quickHeight


    // 一个蓝色矩形组件
    Component{
        id : steelblue
        Rectangle{
            color: "steelblue"
        }
    }

    // 背景布局
    GridLayout
    {
        id:bgLayout
        anchors.fill: parent
        Rectangle{
            id:viewbackground
            color: "steelblue"
            anchors.fill: parent
            z:1
        }
        Text {
            id: title
            text: qsTr("瓦斯继电器智能监测系统")
            z:1
            font.pixelSize: 18
            anchors.top:parent.top
            anchors.horizontalCenter: parent.horizontalCenter

        }

        GridLayout{
            id:leftDisplay
            z:1
            columns:3
            rows:3
            //leftMargin: 5
            anchors.left: parent.left
            width:parent.width/2
            height:width/1.5

            /* 0列 */

            Button{
                id: control
                contentItem: Text {
                    id: name
                    text: qsTr("自动演示(开)")
                    opacity: enabled?1.0:0.3
                    color: control.down ? "black" : "black"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle{
                    implicitWidth: 80
                    implicitHeight: 35
                    radius: 40
                    opacity: enabled ? 1.0 : 0.1
                    border.color: control.down ? "lightcoral" : "red"
                    border.width: 1
                    color:"red"

                }

                property bool onOff : false
                onClicked: {
                    console.debug(control.enabled, control.down)
                    onOff = !onOff
                    if (onOff)
                    {
                        floater.y = Qt.binding(function(){
                                    return floater.parent.height/2-gasValue/15;
                                    });
                        light2Alarm.color = Qt.binding(function(){
                            return gasValue>300&&gasValue<700?"yellow":"blue";
                            });
                        hard2Alarm.color = Qt.binding(function(){
                            return gasValue>=700?"red":"blue";
                            });
                        gasVolumeData.text = Qt.binding(function(){
                            return gasValue;
                            });
                        name.text = "自动演示(关)";
                    }
                    else
                    {
                        floater.y = floater.parent.height/2-gasValue/15;
                        light2Alarm.color = "blue"
                        hard2Alarm.color = "blue"
                        gasVolumeData.text = 0;
                        name.text = "自动演示(开)";
                    }

                }

            }
            /*Rectangle{
                color: "red"
                width: 80
                height: 35
                radius: width/2
                Text {
                    id: title1
                    text: qsTr("自动演示")
                    x:parent.width/2 - width/2
                    y:parent.height/2 - height/2
                    font.pixelSize: 12
                }

                MouseArea{
                    anchors.fill: parent
                    property bool onOff : false
                    onClicked: {
                        onOff = !onOff
                        if (onOff)
                        {
                            floater.y = Qt.binding(function(){
                                        return floater.parent.height/2-gasValue/15;
                                        });
                            light2Alarm.color = Qt.binding(function(){
                                return gasValue>300&&gasValue<700?"yellow":"blue";
                                });
                            hard2Alarm.color = Qt.binding(function(){
                                return gasValue>=700?"red":"blue";
                                });
                            gasVolumeData.text = Qt.binding(function(){
                                return gasValue;
                                });
                        }
                        else
                        {
                            floater.y = floater.parent.height/2-gasValue/15;
                            light2Alarm.color = "blue"
                            hard2Alarm.color = "blue"
                            gasVolumeData.text = 0;
                        }


                    }
                }

            }*/

            Button{
                id: rtDisplay
                Text {
                    id: switchConnect
                    text: qsTr("实时显示")
                    x:parent.width/2 - width/2
                    y:parent.height/2 - height/2
                }
                background: Rectangle{
                    implicitWidth: 80
                    implicitHeight: 35
                    radius: 40
                    opacity: enabled ? 1.0 : 0.1
                    border.color: rtDisplay.down ? "lightcoral" : "red"
                    border.width: 1
                    color:"red"

                }
            }
            Rectangle{
                color: "steelblue"
                width: 80
                height: 35
                radius: width/2
            }


            /* 1列 */
            Rectangle{
                color:"white"
                width: 105
                height: 45
                Text {

                    id: gasVolume
                    x:5
                    text: qsTr("气体体积:\nGas volume")
                    font.pixelSize: 15
                }
            }
            property real gasValue : 0
            Rectangle{
                color: "white"
                width: 105
                height: 45
                radius: 10

                Text {
                    x:parent.width/2 - width/2
                    y:parent.height/2 - height/2
                    font.pixelSize: 15
                    id: gasVolumeData
                    text: "0"
                }
            }

            Text {
                id: unitCM3
                text: qsTr("CM3")
                font.pixelSize: 15
            }

            /* 2列 */
            Rectangle{
                color: "white"
                width: 105
                height: 45
                Text {
                    id: lightAlarm
                    x:5
                    text: qsTr("轻瓦斯报警:\nAlarm switch")
                    font.pixelSize: 15

                }
            }
            Rectangle{
                id: light2Alarm
                color: floater.y>300&&floater.y<700?"yellow":"blue"
                width: 30
                height: 30
                radius: width/2

            }
            Rectangle{
                color: "green"
                width: 30
                height: 30
                radius: width/2
            }

            /* 3列 */
            Rectangle{
                color: "white"
                width: 105
                height: 45
                Text {
                    id: hardAlarm
                    x:5
                    text: qsTr("重瓦斯报警:\nBlock switch")
                    font.pixelSize: 15
                }
            }
            Rectangle{
                id: hard2Alarm
                color: floater.y>700?"red":"blue"
                width: 30
                height: 30
                radius: width/2
            }
            Rectangle{
                color: "green"
                width: 30
                height: 30
                radius: width/2
            }

            /* 4列 */
            Rectangle{
                color:"white"
                width: 140
                height: 45
                Text {
                    id: flowSpeed
                    x:5
                    text: qsTr("油流速度:\nFlow vents speed")
                    font.pixelSize: 15
                }
            }
            Rectangle{
                color: "white"
                width: 100
                height: 45
                radius: 10
            }

            Text {
                id: unitmps
                text: qsTr("m/s")
                font.pixelSize: 15
            }

        }

        GridLayout{

            id:rightAnime
            z:1
            anchors.centerIn: parent
            width:parent.width/2
            height:width/2
            Rectangle{
                id:pot
                color:"gray"
                width:180
                height: 180
                anchors.left: parent.horizontalCenter
                z:1
                radius:20
                Rectangle{
                    id:pole
                    color:"white"
                    width: 18
                    height: 200
                    x:parent.width/2-width/2
                    y:-height/4
                    z:1
                    radius: 3
                }
            }

            Rectangle{
                id:floater
                color: "green"
                width: 80
                height: 40
                x:parent.width/2-width/2; y:0//parent.height/2-gasValue/15

                z: 1
                radius: 5

                Behavior on y{PropertyAnimation {duration: 1000;}}
                Text{
                    text: qsTr("浮子")
                    color: "white"
                    x:29; y:18

                }
                MouseArea{
                    anchors.fill: parent
                    onClicked:{
                        console.debug(bgLayout.parent)
                        console.debug(bgLayout.width)
                        console.debug(leftDisplay.parent.width)
                        console.debug(item1.height)
                        console.debug("leftWidth:",leftDisplay.width,"leftHeight:",leftDisplay.height, "parentWidth:", leftDisplay.parent.width, "parentHeight:", leftDisplay.parent.height)
                        console.debug("rightAnime:", rightAnime.width, "height:", rightAnime.height, "parentW:", rightAnime.parent.width, "H:", rightAnime.parent.height)
                        console.debug("x", floater.x, "parentW", floater.parent.width, "W", floater.width)

                    }
                }

            }
        }



        GridLayout{
            id:bottomIcon
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            z: 1
            Image {
                id: lansoImg
                source: "image/lanso.png"
                anchors.right: parent.right
                anchors.top: parent.top
                scale: 0.5
            }
            Text {
                id: lansoTxt
                text: qsTr("朗松珂利(上海)仪器仪表有限公司")
                font.pixelSize: 13
                anchors.right: parent.right
                anchors.bottom: parent.bottom

            }
        }
    }

}
