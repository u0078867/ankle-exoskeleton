
import React from 'react';
import {GeneratorToPub} from 'react-fast-charts-exp';
import {SubToConsumer} from 'react-fast-charts-exp';


class CommandsDataStream extends React.Component {

    constructor(props) {
        super(props);
        this.state = {
            Ns: 6,
            Ts: 250,
            command: {},
        };
        this.setNs = this.setNs.bind(this);
        this.setTs = this.setTs.bind(this);
    }

    render() {
        return <div>
            <GeneratorToPub
                data={this.state.command}
                dataOutPort="cmd-tune-bd-out"
            />
            <table><tbody>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>Ns: {this.state.Ns}</td>
                                <td><input type="range" min="1" max="20" step="1" value={this.state.Ns} onChange={this.setNs} /></td>
                            </tr>
                            <tr>
                                <td>Ts: {this.state.Ts}</td>
                                <td><input type="range" min="10" max="500" step="10" value={this.state.Ts} onChange={this.setTs} /></td>
                            </tr>
                        </tbody></table>
                    </td>
                    <td>
                        <SubToConsumer dataInPort="cmd-tune-bd-in">
                            <RemoteBufferDigester />
                        </SubToConsumer>
                    </td>
                </tr>
            </tbody></table>
            <br/>
        </div>
    }

    setNs(event) {
        let newValue = parseInt(event.target.value);
        this.setState({Ns: newValue, command: {
            uri: "edit-Ns-Ts",
            payload:{Ns: newValue, Ts: this.state.Ts},
        }});
    }

    setTs(event) {
        let newValue = parseInt(event.target.value);
        this.setState({Ts: newValue, command: {
            uri: "edit-Ns-Ts",
            payload: {Ns: this.state.Ns, Ts: newValue},
        }});
    }

}



class RemoteBufferDigester extends React.Component {

    constructor(props) {
        super(props);
    }

    render() {
        let style = {borderRadius: 5, borderStyle: "solid", borderWidth: 2};
        return <div style={style}>Remote buffer:
            <table><tbody>
                <tr>
                    <td>Ns:</td>
                    <td>{this.props.data.Ns}</td>
                </tr>
                <tr>
                    <td>Ts:</td>
                    <td>{this.props.data.Ts}</td>
                </tr>
            </tbody></table>
        </div>
    }
}

RemoteBufferDigester.defaultProps = {
    data: {Ns: 6, Ts: 250},
}


export default CommandsDataStream;
