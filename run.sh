#create puzzles folder
if [ -d "puzzles" ] 
then
    :
else
    mkdir -p "puzzles";
fi

#generate 5 puzzles
echo "Generating 5 Puzzles"
for i in 1 2 3 4 5
do
    echo "sudoku_puzzle_${i}.jgr"
    ./sudoku CREATE "puzzles/sudoku_puzzle_${i}.jgr"
    ./jgraph "puzzles/sudoku_puzzle_${i}.jgr" | convert -density 300 -quality 100  - "puzzles/sudoku_puzzle_${i}.jpg"
done

#generate 5 solutions to puzzles
echo "Generating Solutions"
for i in 1 2 3 4 5
do
    echo "sudoku_puzzle_${i}_solution.jgr"
    ./sudoku SOLVE "puzzles/sudoku_puzzle_${i}.jgr"
    ./jgraph "puzzles/sudoku_puzzle_${i}_solution.jgr" | convert -density 300 -quality 100  - "puzzles/sudoku_puzzle_${i}_solution.jpg"
done