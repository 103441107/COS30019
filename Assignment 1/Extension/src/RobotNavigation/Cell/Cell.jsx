import React, { Component } from 'react';
import './Cell.css';

export default class Node extends Component {
  handleMouseDown = () => {
    const { row, col, onMouseDown } = this.props;
    onMouseDown(row, col);
  };

  handleMouseEnter = () => {
    const { row, col, onMouseEnter } = this.props;
    onMouseEnter(row, col);
  };

  handleMouseUp = () => {
    const { onMouseUp } = this.props;
    onMouseUp();
  };

  render() {
    const { col, isFinish, isStart, isWall, row } = this.props;
    const extraClassName = isFinish
      ? 'node-finish'
      : isStart
      ? 'node-start'
      : isWall
      ? 'node-wall'
      : '';

    return (
      <div
        id={`node-${row}-${col}`}
        className={`node ${extraClassName}`}
        onMouseDown={this.handleMouseDown}
        onMouseEnter={this.handleMouseEnter}
        onMouseUp={this.handleMouseUp}></div>
    );
  }
}
