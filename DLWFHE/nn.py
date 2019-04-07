import numpy as np
import math
import pandas
import time

class nnetwork(object) :
    def __init__(self, N, M, lr) :
        self.N = N
        self.M = M
        self.W = np.random.random((M, N))
        self.step = lr

    def activef(self, x) :
        return x**2

    def update_step(self, step) :
        self.step = step

    def get_step(self) :
        return self.step

    def encode_w(self, M, E) :
        #print(self.W)
        self.W = M * np.dot(self.W, E)
        #print(self.W)

    def update_w(self, x, t) :
        u = np.dot(self.W,x)
        z = self.activef(u)
        d = self.pr(z) - t
        sz = np.sum(z)

        for i in range(self.M) :
            dw = (-np.sum(d*z/(sz**2)) +d[i]/sz)*(2*u[i]) * x
            self.W[i] = self.W[i] - dw*self.step

#def  traning(self, x, y) :

    def pr(self, z) :
        s = np.sum(z) 
        return z/s        

    def predict(self, x) :
        u = np.dot(self.W, x)
        z = self.activef(u)
        y = self.pr(z)
        return y

    def getW(self) :
        return self.W

    def predictT(self, y) :
        m = np.max(y)
        for i in range(len(y)) :
            if m == y[i] :
                return i

    def training(self, X, Y, epoch) :
        print("Training")
        X = X/(np.max(X))
        num = len(Y) 
        for c in range(epoch) :                        
            print(str(c+1) , end = " : ")
            error = 0
            start = time.clock()
            for i in range(num) :
                x = X[i].T
                t = np.zeros(10).T
                if Y[i] != self.predictT(self.predict(X[i].T)) :
                    error += 1
                t[Y[i]] = 1
                self.update_w(x,t)
            end = time.clock()        
            r = 1- error/num                                              
            print("    accuracy = " + str(r) + "    time = " + str(end - start))


def load_train_data(path) :
    print("Loading train data...")
    data = pandas.DataFrame(pandas.read_csv(path))
    return data.values[:, 0], data.values[:, 1:]


def load_test_data(path) :
    print("Loading test data...")
    data = pandas.DataFrame(pandas.read_csv(path))
    return data.values



def main() :      
    Y, X = load_train_data("train.csv")                        
    epoch = 60       
    net = nnetwork(784, 10, 0.01)
    #在明文上训练
    net.training(X,Y,epoch)

    #在训练集上的准确率
    start = time.clock()         
    Z = []
    for x in X :
        Z.append(net.predictT(net.predict(x)))                               
    num = len(Y)
    error = 0
    end = time.clock()
    print("predict time = " + str(end - start))
    for i in range(num) :
        if Y[i] != Z[i] :
            error += 1
    print("Training accuracy = " + str(1-error/num))

    #在测试集上进行预测
    X_test = load_test_data('test.csv')
    y_test = []
    for x in X_test :
        y_test.append(net.predictT(net.predict(x)))
    print("Writing")
    id = list(range(1, len(y_test)+1))        
    save = pandas.DataFrame({'ImageId': id, 'Label': y_test})
    save = save[['ImageId', 'Label']]
    save.to_csv('submission0.csv',index = False,)

if __name__ == "__main__" :
    main() 