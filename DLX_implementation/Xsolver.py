#!/usr/bin/env python3
# -*- coding: utf-8 -*-
class Xsolver(object):
    def __init__(self,file,size, print_sol=True,write_solution_file=False):
        self.file = file
        self.size = size
        self.print_sol = print_sol
        self.write_solution_file = write_solution_file
        self.X = []
        self.Y = []
        self.grid0 = []
        self.solution = []
        #Class functions
        self.load_sudoku_and_run
        self.solve_sudoku
        self.solve
        self.select
        self.deselect

        
    def load_sudoku_and_run(self):
        import numpy as np
        #Load data
        with open(self.file, 'r') as myfile:
            data=myfile.read().replace('\n', ' ')
        data = data.split()
        #Transform -1 to 0's
        #data = ['0' if x=='-1' else x for x in data]
        #Determine correct sudoku size
        self.size = int(np.sqrt(len(data)))

        for i in range(len(data)):
            data[i] = int(data[i], self.size + 1)
        #Transform data into 0,1,...,16 format
        i=0
        j=0
        self.grid0=[0]*self.size
        while i < len(data):
            self.grid0[j] = data[i:(i + self.size)]
            i += self.size
            j += 1
        grid =np.array(self.grid0)+1
        self.grid0 = grid
        j=0
        # Caller to main functions:
        for solution in self.solve_sudoku():
            if j<1: #Return first solution in case of improper sudoku
                self.solution = solution - 1
                np.set_printoptions(formatter={'int':lambda x:hex(x)[2:]},threshold=np.nan)
                self.solution = np.array(self.solution)
                if self.write_solution_file == True:
                        with open('solution_{}'.format(self.file), 'w') as f:
                            print(self.solution, file=f)
                if self.print_sol==True:
                    print(self.solution)
            else:
                break
            j+=1

    def solve_sudoku(self):
        import numpy as np
        from itertools import product
        #Determine number of rows and columns
        ROWS = int(np.sqrt(self.size))
        COLS = int(np.sqrt(self.size))
        #Define all the values for RowCol, RowNum,ColNum and BoxNum
        self.X = ([("RowCol", rc) for rc in product(range(self.size), range(self.size))] +
             [("RowNum", rn) for rn in product(range(self.size), range(1, self.size + 1))] +
             [("ColNum", cn) for cn in product(range(self.size), range(1, self.size + 1))] +
             [("BoxNum", bn) for bn in product(range(self.size), range(1, self.size + 1))])
        #Define Y dictionary of possible values of RowCol, RowNum,ColNum and BoxNum given 
        # a particular row, col and num value
        self.Y = dict()
        for r, c, n in product(range(self.size), range(self.size), range(1, self.size + 1)):
            b = (r // ROWS) * ROWS + (c // COLS) # Box number
            self.Y[(r, c, n)] = [
                ("RowCol", (r, c)),
                ("RowNum", (r, n)),
                ("ColNum", (c, n)),
                ("BoxNum", (b, n))]
        # Define matrices
        self.X = {j: set() for j in self.X}
        for i, row in self.Y.items():
            for j in row:
                self.X[j].add(i)
        #Use of sudoku hints to discard options in X dict
        for i, row in enumerate(self.grid0):
            for j, n in enumerate(row):
                if n!=0:
                    self.select((i, j, n))
        # Update grid value with true solution
        for solution in self.solve([]):
            for (r, c, n) in solution:
                self.grid0[r][c] = n
            yield self.grid0
    # Solve sudoku recursively until reach a solution (X empty)
    def solve(self,solution_k):
        if not self.X:
            yield list(solution_k)
        else:
            c = min(self.X, key=lambda c: len(self.X[c]))
            for r in list(self.X[c]):
                solution_k.append(r)
                cols = self.select(r)
                # Recursion
                for s in self.solve(solution_k):
                    yield s
                self.deselect(r, cols)
                solution_k.pop()
    # Discard sets using sudoku hints on variable grid0
    def select(self,r):
        cols = []
        for j in self.Y[r]:
            for i in self.X[j]:
                for k in self.Y[i]:
                    if k != j:
                        self.X[k].remove(i)
            cols.append(self.X.pop(j))
        return cols
    # Reverse discard of constraint if no solution was found
    def deselect(self,r,cols):
        for j in reversed(self.Y[r]):
            self.X[j] = cols.pop()
            for i in self.X[j]:
                for k in self.Y[i]:
                    if k != j:
                        self.X[k].add(i)