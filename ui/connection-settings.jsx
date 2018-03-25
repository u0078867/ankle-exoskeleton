
import React from 'react';
import {OutPort} from 'react-fast-charts-exp';
import {InPort} from 'react-fast-charts-exp';
import {WsSocket} from 'react-fast-charts-exp';




class ConnectionSettings extends React.Component {

    constructor(props) {
        super(props);
        this.state = {
            urlStream: "ws://127.0.0.1:5000/",
        };
        this.setUrlStream = this.setUrlStream.bind(this);
    }

    render() {
        let editStyle = {float: "left"};
        let viewStyle = {float: "left", marginLeft: "50px"};
        return <div>
            <div style={editStyle}>
                <table><tbody>
                    <tr>
                        <td>Url:</td>
                        <td><input type="text" value={this.state.urlStream} onChange={this.setUrlStream} /></td>
                    </tr>
                </tbody></table>
            </div>
            <div style={viewStyle}>
                <WsSocket url={this.state.urlStream} statusInPort="ws-status">

                    <InPort port="ws-status" buffered={false} showContent={true} />

                    <InPort port="uc-samples" buffered={true} />

                    <InPort port="cmd-tune-bd-in" buffered={false} />
                    <OutPort port="cmd-tune-bd-out" />

                    <InPort port="cmd-data-saving-in" buffered={false} />
                    <OutPort port="cmd-data-saving-out" />

                    <InPort port="cmd-uC-in" buffered={false} />
                    <OutPort port="cmd-uC-out" />

                </WsSocket>
            </div>
        </div>
    }

    setUrlStream(event) {
        let newUrl = event.target.value;
        this.setState({urlStream: newUrl});
    }

}

export default ConnectionSettings;
