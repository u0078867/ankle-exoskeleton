
import React from 'react';
import ConnectionSettings from './connection-settings';
import CommandsDataStream from './commands-data-stream';
import CommandsDataSaving from './commands-data-saving';
import ControllerTuning from './controller-tuning';
import DataView from './data-view';


class Container extends React.Component {
    render() {
        return <div>
            <table><tbody>
                <tr>
                    <td><ConnectionSettings/></td>
                    <td>
                        <CommandsDataStream/><br/>
                        <CommandsDataSaving/><br/>
                        <ControllerTuning/>
                    </td>
                </tr>
                <tr>
                    <td colSpan="2"><DataView/></td>
                </tr>
            </tbody></table>
        </div>
    }
}



export default Container;
