import math
from math import sqrt
import numbers

def zeroes(height, width):
        """
        Creates a matrix of zeroes.
        """
        g = [[0.0 for _ in range(width)] for __ in range(height)]
        return Matrix(g)

def identity(n):
        """
        Creates a n x n identity matrix.
        """
        I = zeroes(n, n)
        for i in range(n):
            I.g[i][i] = 1.0
        return I

    
def get_row(matrix, row):

    return matrix[row]
    
def get_column(matrix, column_number):
    
    column = []
    for i in range(len(matrix)):
        column.append(matrix[i][column_number])
    return column

def dot_product(vector_one, vector_two):

    result = 0
    for i in range(len(vector_one)):
        result += vector_one[i] * vector_two[i]
    return result

class Matrix(object):

    # Constructor
    def __init__(self, grid):
        self.g = grid
        self.h = len(grid)
        self.w = len(grid[0])

    #
    # Primary matrix math methods
    #############################
 
    def determinant(self):
        """
        Calculates the determinant of a 1x1 or 2x2 matrix.
        """
       
        if not self.is_square():
            raise(ValueError, "Cannot calculate determinant of non-square matrix.")
        if self.h > 2:
            raise(NotImplementedError, "Calculating determinant not implemented for matrices largerer than 2x2.")
        
        det = 0 
        
        if (self.h == 1 and self.w == 1 ):
            return self.g[0][0]
        
        else: 
            det =  self.g[0][0] * self.g[1][1] - self.g[0][1] * self.g[1][0] 
            return det
            

    def trace(self):
        """
        Calculates the trace of a matrix (sum of diagonal entries).
        """
        if not self.is_square():
            raise(ValueError, "Cannot calculate the trace of a non-square matrix.")

        # TODO - your code here
        # The Sum of the trace 
        S = 0 
        for i in range(self.h):
                S = S + self.g[i][i]
                
        return S 

    def inverse(self):
        """
        Calculates the inverse of a 1x1 or 2x2 Matrix.
        """
        if not self.is_square():
            raise(ValueError, "Non-square Matrix does not have an inverse.")
        if self.h > 2:
            raise(NotImplementedError, "inversion not implemented for matrices larger than 2x2.")
        
        inverse = zeroes( self.h, self.w )
        
        # TODO - your code here
        # Check if matrix is 1x1 or 2x2.
        # Depending on the matrix size, the formula for calculating
        # the inverse is different. 
        
        
        # For Matrix 1x1
        if((self.h * self.w) == 1):
            
            inv_determinant = 1 / self.determinant()
            inverse[0][0] = inv_determinant
            
        elif ( self.h == 2 and self.w ==2 ): 
            # If the matrix is 2x2, check that the matrix is invertible
            if self.determinant()==0:
                raise ValueError('The matrix is not invertible.')
            else:
                # Calculate the inverse of the square 1x1 or 2x2 matrix.
                a = self.g[0][0]
                b = self.g[0][1]
                c = self.g[1][0]
                d = self.g[1][1]

                factor = 1 / (a * d - b * c)

                inverse_f = [[d, -b],[-c, a]]

                for i in range(inverse.h):
                    for j in range(inverse.w):
                        inverse[i][j] =factor * inverse_f[i][j]
        return inverse

    def T(self):
        """
        Returns a transposed copy of this Matrix.
        """
        matrix_transpose = zeroes( self.w, self.h )
        # Loop through columns on outside loop
        for c in range(self.h):
            # Loop through columns on inner loop
            for r in range(self.w):
            # Column values will be filled by what were each row before
                matrix_transpose.g[r][c] = self.g[c][r]
                            
        return matrix_transpose

    def is_square(self):
        return self.h == self.w

    #
    # Begin Operator Overloading
    ############################
    def __getitem__(self,idx):
        """
        Defines the behavior of using square brackets [] on instances
        of this class.

        Example:

        > my_matrix = Matrix([ [1, 2], [3, 4] ])
        > my_matrix[0]
          [1, 2]

        > my_matrix[0][0]
          1
        """
        return self.g[idx]

    def __repr__(self):
        """
        Defines the behavior of calling print on an instance of this class.
        """
        s = ""
        for row in self.g:
            s += " ".join(["{} ".format(x) for x in row])
            s += "\n"
        return s

    def __add__(self,other):
        """
        Defines the behavior of the + operator
        """
        if self.h != other.h or self.w != other.w:
            raise(ValueError, "Matrices can only be added if the dimensions are the same") 
        #   
        # TODO - your code here
        #
        Ans = [] 
        
        for i in range(self.h):
            
            row = [] 
            
            for j in range(self.w): 
                row.append(self.g[i][j] + other.g[i][j])

            Ans.append(row)
        
        return Matrix(Ans)

    def __neg__(self):
        """
        Defines the behavior of - operator (NOT subtraction)

        Example:

        > my_matrix = Matrix([ [1, 2], [3, 4] ])
        > negative  = -my_matrix
        > print(negative)
          -1.0  -2.0
          -3.0  -4.0
        """
        #   
        # TODO - your code here
        #
        neg_matrix = []
        for i in range(self.h):
            row = [] 
            for j in range(self.w):
                row.append(-1*self[i][j])
            neg_matrix.append(row)
            
        return Matrix(neg_matrix)

    def __sub__(self, other):
        """
        Defines the behavior of - operator (as subtraction)
        """
        subt_matrix = []
        for i in range(self.h):
            row = [] 
            for j in range(self.w):
                row.append( self.g[i][j] - other.g[i][j])
            subt_matrix.append(row)
            
        return Matrix(subt_matrix)
            

    def __mul__(self, other):
        """
        Defines the behavior of * operator (matrix multiplication)
        """
        #   
        # TODO - your code here
        #
        
        # Get dimensions of the Matrix 1 and the Matrix 2
        m_rows = self.h
        p_columns = other.w
        
        # empty list that will hold the product of AxB
        matrixMul = []
        
        for i in range(m_rows):
            row = []
            for j in range(p_columns):
                row.append(dot_product(get_row(self.g,i),get_column(other.g,j)))
            matrixMul.append(row)
        return Matrix(matrixMul)

    def __rmul__(self, other):
        """
        Called when the thing on the left of the * is not a matrix.

        Example:

        > identity = Matrix([ [1,0], [0,1] ])
        > doubled  = 2 * identity
        > print(doubled)
          2.0  0.0
          0.0  2.0
        """
        ans = [] 
        if isinstance(other, numbers.Number):
            pass
        
            for i in range(self.h):
                row = []
                for j in range(self.w):
                     row.append(self.g[i][j] * other)
                ans.append(row)
            
            return Matrix(ans)
            