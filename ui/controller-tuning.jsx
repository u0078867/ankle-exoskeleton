
import React from 'react';
import {GeneratorToPub} from 'react-fast-charts-exp';
import {SubToConsumer} from 'react-fast-charts-exp';


class ControllerTuning extends React.Component {

    constructor(props) {
        super(props);
        this.state = {
            // settings
            MECHBLOCK: 0,
            SERVOBEH: 1,
            // parameters
            th: 0.0,
            ALGMOD: 0,
            FVANTHS: 0.0,
            VACTHS: 0.0,
            FVANTTO: 0.0,
            VACTTO: 0.0,
            HOMOD: 1,
            FVANTHO: 0.0,
            SVANTHO: 0.0,
            TWNOHO: 0,
            MAD: 0.0,
            MAP: 0.0,
            MASW: 0.0,
            E2R: 0,
            E3R: 0,
            E1SW: 0,
            AANE: 0.0,
            ST: 0,
        }
        this.sender = new GeneratorToPub({dataOutPort: 'cmd-uC-out'});
        this.initSettings = this.initSettings.bind(this);
        this.handleControllerMessage = this.handleControllerMessage.bind(this);
        this.setTh = this.setTh.bind(this);
        this.setMECHBLOCK = this.setMECHBLOCK.bind(this);
        this.setSERVOBEH = this.setSERVOBEH.bind(this);
        this.setALGMOD = this.setALGMOD.bind(this);
        this.setFVANTHS = this.setFVANTHS.bind(this);
        this.setVACTHS = this.setVACTHS.bind(this);
        this.setFVANTTO = this.setFVANTTO.bind(this);
        this.setVACTTO = this.setVACTTO.bind(this);
        this.setHOMOD = this.setHOMOD.bind(this);
        this.setFVANTHO = this.setFVANTHO.bind(this);
        this.setSVANTHO = this.setSVANTHO.bind(this);
        this.setTWNOHO = this.setTWNOHO.bind(this);
        this.setMAD = this.setMAD.bind(this);
        this.setMAP = this.setMAP.bind(this);
        this.setMASW = this.setMASW.bind(this);
        this.setE2R = this.setE2R.bind(this);
        this.setE3R = this.setE3R.bind(this);
        this.setE1SW = this.setE1SW.bind(this);
        this.setAANE = this.setAANE.bind(this);
        this.setST = this.setST.bind(this);
    }

    initSettings(data) {
        if (data == 'ws-connected') {
            /*this.sender.send({
                uri: "edit-uC-th",
                payload: this.state.th,
            });*/
        }
    }

    handleControllerMessage(data) {
        // Add message to list
        this.refs.controllerMessage.value = this.refs.controllerMessage.value +
                                            (new Date()).toLocaleTimeString('en-GB') + ' --- ' + data.param + ': ' + data.msg + "\n";
        this.refs.controllerMessage.scrollTop = this.refs.controllerMessage.scrollHeight; // to make scroll stay at the bottom
        // Show received param
        let idx = data.msg.search("Read");
        if (idx >= 0) {
            let value = data.msg.slice(idx+5);
            console.log('set'+data.param);
            this['set'+data.param]({target: {value: value}, type: 'external'});
        }
    }

    render() {
        return <div>
            <SubToConsumer dataInPort="ws-status" receiveCallback={this.initSettings} />
            <SubToConsumer dataInPort="cmd-uC-in" receiveCallback={this.handleControllerMessage} />
            <table><tbody>
                <tr>
                    <td>
                        {/*<table><tbody>
                            <tr>
                                <td>Threshold: </td>
                                <td><input type="number" min="0" max="2" step="0.01" onBlur={this.setTh} /></td>
                            </tr>
                        </tbody></table>*/}
                        <b>Settings:</b>
                    </td>
                    <td rowSpan="21">
                        Controller says:<br/>
                        <textarea ref="controllerMessage" style={{"height":"500px", "width":"300px", "overflow":"scroll", "resize":"none"}} readOnly>
                        </textarea>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>MECHBLOCK: </td>
                                <td>
                                    <select value={this.state.MECHBLOCK} onChange={this.setMECHBLOCK}>
                                        <option value="0">Do not block on mech limits</option>
                                        <option value="1">Block on mech limits</option>
                                    </select>
                                </td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>SERVOBEH: </td>
                                <td>
                                    <select value={this.state.SERVOBEH} onChange={this.setSERVOBEH}>
                                        <option value="1">Attach/detach+repower</option>
                                        <option value="2">Attach/detach</option>
                                        <option value="3">Power/unpower</option>
                                    </select>
                                </td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <b>Algorithm parameters:</b>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>ALGMOD: </td>
                                <td>
                                    <select value={this.state.ALGMOD} onChange={this.setALGMOD}>
                                        <option value="0">Stopped</option>
                                        <option value="1">Active</option>
                                    </select>
                                </td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>FVANTHS: </td>
                                <td><input type="number" min="0" max="50" onBlur={this.setFVANTHS} ref="FVANTHS" /></td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>VACTHS: </td>
                                <td><input type="number" min="0" max="100" onBlur={this.setVACTHS} ref="VACTHS" /></td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>FVANTTO: </td>
                                <td><input type="number" min="-50" max="0" onBlur={this.setFVANTTO} ref="FVANTTO" /></td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>VACTTO: </td>
                                <td><input type="number" min="0" max="100" onBlur={this.setVACTTO} ref="VACTTO" /></td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>HOMOD: </td>
                                <td>
                                    <select value={this.state.HOMOD} onChange={this.setHOMOD}>
                                        <option value="1">FVAN/AAN-based</option>
                                        <option value="2">SVAN-based (1)</option>
                                        <option value="3">SVAN-based (2)</option>
                                        <option value="4">FVAN-based</option>
                                    </select>
                                </td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>FVANTHO: </td>
                                <td><input type="number" min="-10" max="40" onBlur={this.setFVANTHO} ref="FVANTHO" /></td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>SVANTHO: </td>
                                <td><input type="number" min="-5" max="30" onBlur={this.setSVANTHO} ref="SVANTHO" /></td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>TWNOHO: </td>
                                <td><input type="number" min="0" max="1000" onBlur={this.setTWNOHO} ref="TWNOHO" /></td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>MAD: </td>
                                <td><input type="number" min="0" max="30" onBlur={this.setMAD} ref="MAD" /></td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>MAP: </td>
                                <td><input type="number" min="-30" max="1" onBlur={this.setMAP} ref="MAP" /></td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>MASW: </td>
                                <td><input type="number" min="-10" max="30" onBlur={this.setMASW} ref="MASW" /></td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>E2R: </td>
                                <td><input type="number" min="100" max="3000" onBlur={this.setE2R} ref="E2R" /></td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>E3R: </td>
                                <td><input type="number" min="300" max="5000" onBlur={this.setE3R} ref="E3R" /></td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>E1SW: </td>
                                <td><input type="number" min="300" max="5000" onBlur={this.setE1SW} ref="E1SW" /></td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>AANE: </td>
                                <td><input type="number" min="0" max="5" onBlur={this.setAANE} ref="AANE" /></td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>ST: </td>
                                <td><input type="number" min="300" max="10000" onBlur={this.setST} ref="ST" /></td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
            </tbody></table>
            <br/>
        </div>
    }

    setTh(event) {
        if (parseFloat(event.target.value) > parseFloat(event.target.max)) {
            event.target.value = event.target.max;
        }
        if (parseFloat(event.target.value) < parseFloat(event.target.min)) {
            event.target.value = event.target.min;
        }
        let newValue = event.target.value;
        this.setState({th: parseFloat(newValue)});
        if (newValue == '') return;
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-th",
                payload: newValue,
            });
        }
    }

    setALGMOD(event) {
        let newValue = event.target.value;
        this.setState({ALGMOD: parseInt(newValue)});
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-ALGMOD",
                payload: newValue,
            });
        }
    }

    setFVANTHS(event) {
        if (parseFloat(event.target.value) > parseFloat(event.target.max)) {
            event.target.value = event.target.max;
        }
        if (parseFloat(event.target.value) < parseFloat(event.target.min)) {
            event.target.value = event.target.min;
        }
        let newValue = event.target.value;
        this.setState({FVANTHS: parseFloat(newValue)});
        if (newValue == '') return;
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-FVANTHS",
                payload: newValue,
            });
        } else {
            this.refs.FVANTHS.value = parseFloat(newValue);
        }
    }

    setVACTHS(event) {
        if (parseFloat(event.target.value) > parseFloat(event.target.max)) {
            event.target.value = event.target.max;
        }
        if (parseFloat(event.target.value) < parseFloat(event.target.min)) {
            event.target.value = event.target.min;
        }
        let newValue = event.target.value;
        this.setState({VACTHS: parseFloat(newValue)});
        if (newValue == '') return;
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-VACTHS",
                payload: newValue,
            });
        } else {
            this.refs.VACTHS.value = parseFloat(newValue);
        }
    }

    setFVANTTO(event) {
        if (parseFloat(event.target.value) > parseFloat(event.target.max)) {
            event.target.value = event.target.max;
        }
        if (parseFloat(event.target.value) < parseFloat(event.target.min)) {
            event.target.value = event.target.min;
        }
        let newValue = event.target.value;
        this.setState({FVANTTO: parseFloat(newValue)});
        if (newValue == '') return;
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-FVANTTO",
                payload: newValue,
            });
        } else {
            this.refs.FVANTTO.value = parseFloat(newValue);
        }
    }

    setVACTTO(event) {
        if (parseFloat(event.target.value) > parseFloat(event.target.max)) {
            event.target.value = event.target.max;
        }
        if (parseFloat(event.target.value) < parseFloat(event.target.min)) {
            event.target.value = event.target.min;
        }
        let newValue = event.target.value;
        this.setState({VACTTO: parseFloat(newValue)});
        if (newValue == '') return;
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-VACTTO",
                payload: newValue,
            });
        } else {
            this.refs.VACTTO.value = parseFloat(newValue);
        }
    }

    setHOMOD(event) {
        let newValue = event.target.value;
        this.setState({HOMOD: parseInt(newValue)});
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-HOMOD",
                payload: newValue,
            });
        }
    }

    setFVANTHO(event) {
        if (parseFloat(event.target.value) > parseFloat(event.target.max)) {
            event.target.value = event.target.max;
        }
        if (parseFloat(event.target.value) < parseFloat(event.target.min)) {
            event.target.value = event.target.min;
        }
        let newValue = event.target.value;
        this.setState({FVANTHO: parseFloat(newValue)});
        if (newValue == '') return;
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-FVANTHO",
                payload: newValue,
            });
        } else {
            this.refs.FVANTHO.value = parseFloat(newValue);
        }
    }

    setSVANTHO(event) {
        if (parseFloat(event.target.value) > parseFloat(event.target.max)) {
            event.target.value = event.target.max;
        }
        if (parseFloat(event.target.value) < parseFloat(event.target.min)) {
            event.target.value = event.target.min;
        }
        let newValue = event.target.value;
        this.setState({SVANTHO: parseFloat(newValue)});
        if (newValue == '') return;
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-SVANTHO",
                payload: newValue,
            });
        } else {
            this.refs.SVANTHO.value = parseFloat(newValue);
        }
    }

    setTWNOHO(event) {
        if (parseInt(event.target.value) > parseInt(event.target.max)) {
            event.target.value = event.target.max;
        }
        if (parseInt(event.target.value) < parseInt(event.target.min)) {
            event.target.value = event.target.min;
        }
        let newValue = event.target.value;
        this.setState({TWNOHO: parseInt(newValue)});
        if (newValue == '') return;
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-TWNOHO",
                payload: newValue,
            });
        } else {
            this.refs.TWNOHO.value = parseInt(newValue);
        }
    }

    setMAD(event) {
        if (parseFloat(event.target.value) > parseFloat(event.target.max)) {
            event.target.value = event.target.max;
        }
        if (parseFloat(event.target.value) < parseFloat(event.target.min)) {
            event.target.value = event.target.min;
        }
        let newValue = event.target.value;
        this.setState({MAD: parseFloat(newValue)});
        if (newValue == '') return;
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-MAD",
                payload: newValue,
            });
        } else {
            this.refs.MAD.value = parseFloat(newValue);
        }
    }

    setMAP(event) {
        if (parseFloat(event.target.value) > parseFloat(event.target.max)) {
            event.target.value = event.target.max;
        }
        if (parseFloat(event.target.value) < parseFloat(event.target.min)) {
            event.target.value = event.target.min;
        }
        let newValue = event.target.value;
        this.setState({MAP: parseFloat(newValue)});
        if (newValue == '') return;
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-MAP",
                payload: newValue,
            });
        } else {
            this.refs.MAP.value = parseFloat(newValue);
        }
    }

    setMASW(event) {
        if (parseFloat(event.target.value) > parseFloat(event.target.max)) {
            event.target.value = event.target.max;
        }
        if (parseFloat(event.target.value) < parseFloat(event.target.min)) {
            event.target.value = event.target.min;
        }
        let newValue = event.target.value;
        this.setState({MASW: parseFloat(newValue)});
        if (newValue == '') return;
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-MASW",
                payload: newValue,
            });
        } else {
            this.refs.MASW.value = parseFloat(newValue);
        }
    }

    setE2R(event) {
        if (parseInt(event.target.value) > parseInt(event.target.max)) {
            event.target.value = event.target.max;
        }
        if (parseInt(event.target.value) < parseInt(event.target.min)) {
            event.target.value = event.target.min;
        }
        let newValue = event.target.value;
        this.setState({E2R: parseInt(newValue)});
        if (newValue == '') return;
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-E2R",
                payload: newValue,
            });
        } else {
            this.refs.E2R.value = parseInt(newValue);
        }
    }

    setE3R(event) {
        if (parseInt(event.target.value) > parseInt(event.target.max)) {
            event.target.value = event.target.max;
        }
        if (parseInt(event.target.value) < parseInt(event.target.min)) {
            event.target.value = event.target.min;
        }
        let newValue = event.target.value;
        this.setState({E3R: parseInt(newValue)});
        if (newValue == '') return;
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-E3R",
                payload: newValue,
            });
        } else {
            this.refs.E3R.value = parseInt(newValue);
        }
    }

    setE1SW(event) {
        if (parseInt(event.target.value) > parseInt(event.target.max)) {
            event.target.value = event.target.max;
        }
        if (parseInt(event.target.value) < parseInt(event.target.min)) {
            event.target.value = event.target.min;
        }
        let newValue = event.target.value;
        this.setState({E1SW: parseInt(newValue)});
        if (newValue == '') return;
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-E1SW",
                payload: newValue,
            });
        } else {
            this.refs.E1SW.value = parseInt(newValue);
        }
    }

    setAANE(event) {
        if (parseFloat(event.target.value) > parseFloat(event.target.max)) {
            event.target.value = event.target.max;
        }
        if (parseFloat(event.target.value) < parseFloat(event.target.min)) {
            event.target.value = event.target.min;
        }
        let newValue = event.target.value;
        this.setState({AANE: parseFloat(newValue)});
        if (newValue == '') return;
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-AANE",
                payload: newValue,
            });
        } else {
            this.refs.AANE.value = parseFloat(newValue);
        }
    }

    setMECHBLOCK(event) {
        let newValue = event.target.value;
        this.setState({MECHBLOCK: parseInt(newValue)});
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-MECHBLOCK",
                payload: newValue,
            });
        }
    }

    setSERVOBEH(event) {
        let newValue = event.target.value;
        this.setState({SERVOBEH: parseInt(newValue)});
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-SERVOBEH",
                payload: newValue,
            });
        }
    }

    setST(event) {
        if (parseInt(event.target.value) > parseInt(event.target.max)) {
            event.target.value = event.target.max;
        }
        if (parseInt(event.target.value) < parseInt(event.target.min)) {
            event.target.value = event.target.min;
        }
        let newValue = event.target.value;
        this.setState({ST: parseInt(newValue)});
        if (newValue == '') return;
        if (event.type != 'external') {
            this.sender.send({
                uri: "edit-uC-ST",
                payload: newValue,
            });
        } else {
            this.refs.ST.value = parseInt(newValue);
        }
    }

}


export default ControllerTuning;
