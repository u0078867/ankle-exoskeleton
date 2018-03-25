
import React from 'react';
import {Chart2Dd3 as Chart2D} from 'react-fast-charts-exp';
import {SubToConsumer} from 'react-fast-charts-exp';
import {DataDisplay} from 'react-fast-charts-exp';

class DataView extends React.Component {

    constructor(props) {
        super(props);
        this.state = {
            showCharts: true,
        };
        this.setShowCharts = this.setShowCharts.bind(this);
    }

    render() {
        let width = 400;
        let height = 300;
        let margins = {left: 50, right: 50, top: 50, bottom: 50};
        //let x = "frameNumber";
        let x = "TS";
        let color = ["blue"];
        let bufferLength = 200;
        let buffered = true;
        let showCharts = this.state.showCharts;
        let charts = <div></div>;
        if (showCharts) {
            charts = <table><tbody>
                <tr>
                    <td>
                        <Chart2D
                            title="Motor torque"
                            fieldX={x}
                            fieldY={["MT"]}
                            yAxisLabel="(Nm)"
                            color={color}
                            bufferLength={bufferLength}
                            buffered={buffered}
                            width={width}
                            height={height}
                            margins={margins}
                        />
                    </td>
                    <td>
                        <Chart2D
                            title="Motor position"
                            fieldX={x}
                            fieldY={["MP"]}
                            yAxisLabel="(deg)"
                            color={color}
                            bufferLength={bufferLength}
                            buffered={buffered}
                            width={width}
                            height={height}
                            margins={margins}
                        />
                    </td>
                    <td>
                        <Chart2D
                            title="Motor target position"
                            fieldX={x}
                            fieldY={["MTP"]}
                            yAxisLabel="(deg)"
                            color={color}
                            bufferLength={bufferLength}
                            buffered={buffered}
                            width={width}
                            height={height}
                            margins={margins}
                        />
                    </td>
                </tr>
                <tr>
                    <td>
                        <Chart2D
                            title="Shank angle with vertical"
                            fieldX={x}
                            fieldY={["SVAN"]}
                            yAxisLabel="(deg)"
                            color={color}
                            bufferLength={bufferLength}
                            buffered={buffered}
                            width={width}
                            height={height}
                            margins={margins}
                        />
                    </td>
                    <td>
                        <Chart2D
                            title="Ankle angle (target)"
                            fieldX={x}
                            fieldY={["AAN","AANT"]}
                            yAxisLabel="(deg)"
                            color={["blue","red"]}
                            bufferLength={bufferLength}
                            buffered={buffered}
                            width={width}
                            height={height}
                            margins={margins}
                        />
                    </td>
                    <td>
                        <Chart2D
                            title="Foot angle with vertical"
                            fieldX={x}
                            fieldY={["FVAN"]}
                            yAxisLabel="(deg)"
                            color={color}
                            bufferLength={bufferLength}
                            buffered={buffered}
                            width={width}
                            height={height}
                            margins={margins}
                        />
                    </td>
                </tr>
                <tr>
                    <td>
                        <Chart2D
                            title="Spring compression"
                            fieldX={x}
                            fieldY={["SPC"]}
                            yAxisLabel="(mm)"
                            color={color}
                            bufferLength={bufferLength}
                            buffered={buffered}
                            width={width}
                            height={height}
                            margins={margins}
                        />
                    </td>
                    <td>
                        <Chart2D
                            title="IMU vertical acceleration"
                            fieldX={x}
                            fieldY={["VAC"]}
                            yAxisLabel="(m/s^2)"
                            color={color}
                            bufferLength={bufferLength}
                            buffered={buffered}
                            width={width}
                            height={height}
                            margins={margins}
                        />
                    </td>
                </tr>
            </tbody></table>
        }
        return <div>
            <div></div>
            <table><tbody>
                <tr>
                    <td>Show charts:</td>
                    <td><input type="checkbox" checked={this.state.showCharts} onChange={this.setShowCharts} /></td>
                </tr>
            </tbody></table>
            <SubToConsumer dataInPort="uc-samples">
                <DataDisplay dataOrder={["SIDE","TS","MT","MP","MTP","SVAN","AAN","AANT","FVAN","SPC","SVAC","MDET","GP"]}/>
                {charts}
            </SubToConsumer>
        </div>
    }

    setShowCharts(event) {
        this.setState({showCharts: !this.state.showCharts});
    }

}

export default DataView;
