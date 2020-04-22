# import pdb
from helpers import normalize, blur

def initialize_beliefs(grid):
    height = len(grid)
    width = len(grid[0])
    area = height * width
    belief_per_cell = 1.0 / area
    beliefs = []
    for i in range(height):
        row = []
        for j in range(width):
            row.append(belief_per_cell)
        beliefs.append(row)
    return beliefs

def sense(color, grid, beliefs, p_hit, p_miss):
    new_beliefs = []
    
    # Mult p_hit for matching cell and p_miss for un-matching cell
    row = len(grid)
    col = len(grid[0])
    S = 0 
    
    for i in range(row): 
        row_temp = []
        
        for j in range (col): 
 
            # Check the color for each cell 
            hit = (grid[i][j]==color)
            temp_val = beliefs[i][j] * ( p_hit * hit + p_miss * (1-hit) )
            row_temp.append(temp_val) 
            S = S + temp_val
                            
        new_beliefs.append(row_temp)
    
    for i in range(row): 
        for j in range(col): 
            new_beliefs[i][j] = (new_beliefs[i][j])/S

    return new_beliefs

def move(dy, dx, beliefs, blurring):
    height = len(beliefs)
    width = len(beliefs[0])
    new_G = [[0.0 for i in range(width)] for j in range(height)]
    for i, row in enumerate(beliefs):
        for j, cell in enumerate(row):
            
            # There was error in the height and wisth positions in new_i  & new_j; sol : change them interchangably 
            new_i = (i + dy ) % height
            new_j = (j + dx ) % width
            # pdb.set_trace()
            new_G[int(new_i)][int(new_j)] = cell
    return blur(new_G, blurring)