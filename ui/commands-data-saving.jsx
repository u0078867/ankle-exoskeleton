
import React from 'react';
import {GeneratorToPub} from 'react-fast-charts-exp';
import {SubToConsumer} from 'react-fast-charts-exp';


class CommandsDataSaving extends React.Component {

    constructor(props) {
        super(props);
        this.state = {
            folderMain: "",
            fileBasename: "Trial",
            fileNumber: 1,
            fileSaving: false,
        }
        this.sender = new GeneratorToPub({dataOutPort: 'cmd-data-saving-out'}); // note the use of the React component as non-React object
        this.initSettings = this.initSettings.bind(this);
        this.handleFileSavingMessage = this.handleFileSavingMessage.bind(this);
        this.setFolderMain = this.setFolderMain.bind(this);
        this.setFileBasename = this.setFileBasename.bind(this);
        this.setFileNumber = this.setFileNumber.bind(this);
        this.setFileSaving = this.setFileSaving.bind(this);
    }

    initSettings(data) {
        if (data == 'ws-connected') {
            this.sender.send({
                uri: "edit-folder-main",
                payload: this.state.folderMain,
            });
            this.sender.send({
                uri: "edit-file-name",
                payload: this.state.fileBasename + ' ' + this.state.fileNumber,
            });
        }
    }

    handleFileSavingMessage(data) {
        let newValue;
        if (data == 'file-saving') {
            newValue = true;
        } else if (data == 'no-file-saving') {
            newValue = false;
        }
        this.setState({fileSaving: newValue});
        if (newValue == false) {
            this.setFileNumber({target: {value: (parseInt(this.state.fileNumber) + 1).toString()}});
        }
    }

    render() {
        return <div>
            <SubToConsumer dataInPort="ws-status" receiveCallback={this.initSettings} />
            <SubToConsumer dataInPort="cmd-data-saving-in" receiveCallback={this.handleFileSavingMessage} />
            <table><tbody>
                <tr>
                    <td>
                        <table><tbody>
                            <tr>
                                <td>Remote main folder: </td>
                                <td><input type="text" value={this.state.folderMain} onChange={this.setFolderMain} /></td>
                            </tr>
                            <tr>
                                <td>Remote file basename: </td>
                                <td><input type="text" value={this.state.fileBasename} onChange={this.setFileBasename} /></td>
                            </tr>
                            <tr>
                                <td>Remote file number: </td>
                                <td><input type="number" min="1" value={this.state.fileNumber} onChange={this.setFileNumber} /></td>
                            </tr>
                            <tr>
                                <td>Remote file saving: </td>
                                <td><button onClick={this.setFileSaving}>{this.state.fileSaving? "Stop saving": "Start saving"}</button></td>
                            </tr>
                            <tr>
                                <td>Remote file saving status: </td>
                                <td>{this.state.fileSaving? "File saving ...": "No file saving"}</td>
                            </tr>
                        </tbody></table>
                    </td>
                </tr>
            </tbody></table>
            <br/>
        </div>
    }

    setFolderMain(event) {
        let newValue = event.target.value;
        this.sender.send({
            uri: "edit-folder-main",
            payload: newValue,
        });
        this.setState({folderMain: newValue});
    }

    setFileBasename(event) {
        let newValue = event.target.value;
        this.setState({fileBasename: newValue});
        this.sender.send({
            uri: "edit-file-name",
            payload: newValue + ' ' + this.state.fileNumber,
        });
    }

    setFileNumber(event) {
        let newValue = event.target.value;
        this.setState({fileNumber: newValue});
        this.sender.send({
            uri: "edit-file-name",
            payload: this.state.fileBasename + ' ' + newValue,
        });
    }

    setFileSaving(event) {
        let newValue = !this.state.fileSaving;
        this.setState({fileSaving: newValue});
        this.sender.send({
            uri: "edit-file-saving",
            payload: newValue,
        });
        if (newValue == false) {
            this.setFileNumber({target: {value: (parseInt(this.state.fileNumber) + 1).toString()}});
        }
    }


}


export default CommandsDataSaving;
