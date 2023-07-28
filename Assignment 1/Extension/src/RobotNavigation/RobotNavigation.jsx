import React, { Component } from 'react';
import Node from './Cell/Cell';
import { dijkstraPath, getNodesInShortestPathOrder } from '../algorithms/dijkstra';

import './RobotNavigation.css';

const START_NODE_ROW = 15;
const START_NODE_COL = 10;
const FINISH_NODE_ROW = 5;
const FINISH_NODE_COL = 49;

const getInitialGrid = () => {
  const grid = [];
  for (let row = 0; row < 20; row++) {
    const currentRow = [];
    for (let col = 0; col < 60; col++) {
      currentRow.push(createNode(col, row));
    }
    grid.push(currentRow);
  }
  return grid;
};

const createNode = (col, row) => ({
  col,
  row,
  isStart: row === START_NODE_ROW && col === START_NODE_COL,
  isFinish: row === FINISH_NODE_ROW && col === FINISH_NODE_COL,
  distance: Infinity,
  isVisited: false,
  isWall: false,
  previousNode: null,
});

const getNewGridWithWallToggled = (grid, row, col) => {
  const newGrid = [...grid];
  const node = newGrid[row][col];
  const newNode = { ...node, isWall: !node.isWall };
  newGrid[row][col] = newNode;
  return newGrid;
};

export default class RobotNavigation extends Component {
  state = {
    grid: [],
    mouseIsPressed: false,
  };

  componentDidMount() {
    const grid = getInitialGrid();
    this.setState({ grid });
  }

  handleMouseDown(row, col) {
    const newGrid = getNewGridWithWallToggled(this.state.grid, row, col);
    this.setState({ grid: newGrid, mouseIsPressed: true });
  }

  handleMouseEnter(row, col) {
    if (!this.state.mouseIsPressed) return;
    const newGrid = getNewGridWithWallToggled(this.state.grid, row, col);
    this.setState({ grid: newGrid });
  }

  handleMouseUp() {
    this.setState({ mouseIsPressed: false });
  }

  animateDijkstra(visitedNodesInOrder, nodesInShortestPathOrder) {
    visitedNodesInOrder.forEach((node, i) => {
      setTimeout(() => {
        const nodeElement = document.getElementById(`node-${node.row}-${node.col}`);
        if (nodeElement) {
          nodeElement.className = 'node node-visited';
        }
      }, 10 * i);
    });

    setTimeout(() => {
      this.animateShortestPath(nodesInShortestPathOrder);
    }, 10 * visitedNodesInOrder.length);
  }

  animateShortestPath(nodesInShortestPathOrder) {
    nodesInShortestPathOrder.forEach((node, i) => {
      setTimeout(() => {
        const nodeElement = document.getElementById(`node-${node.row}-${node.col}`);
        if (nodeElement) {
          nodeElement.className = 'node node-shortest-path';
        }
      }, 50 * i);
    });
  }

  visualizeDijkstra() {
    const { grid } = this.state;
    const startNode = grid[START_NODE_ROW][START_NODE_COL];
    const finishNode = grid[FINISH_NODE_ROW][FINISH_NODE_COL];
    const visitedNodesInOrder = dijkstraPath(grid, startNode, finishNode);
    const nodesInShortestPathOrder = getNodesInShortestPathOrder(finishNode);
    this.animateDijkstra(visitedNodesInOrder, nodesInShortestPathOrder);
  }

  resetDijkstra() {
    window.location.reload();
  }

  render() {
    const { grid, mouseIsPressed } = this.state;

    return (
      <>
        <button className="button" onClick={() => this.visualizeDijkstra()}>
          Visualize Dijkstra's Algorithm
        </button>
        <button className="button-reset" onClick={() => this.resetDijkstra()}>
          Reset
        </button>

        <div className="grid">
          {grid.map((row, rowIdx) => (
            <div key={rowIdx}>
              {row.map((node, nodeIdx) => {
                const { row, col, isFinish, isStart, isWall } = node;
                return (
                  <Node
                    key={nodeIdx}
                    col={col}
                    isFinish={isFinish}
                    isStart={isStart}
                    isWall={isWall}
                    mouseIsPressed={mouseIsPressed}
                    onMouseDown={() => this.handleMouseDown(row, col)}
                    onMouseEnter={() => this.handleMouseEnter(row, col)}
                    onMouseUp={() => this.handleMouseUp()}
                    row={row}
                  />
                );
              })}
            </div>
          ))}
        </div>
      </>
    );
  }
}
