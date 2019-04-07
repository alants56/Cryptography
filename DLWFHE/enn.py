import numpy as np
import math
import pandas
import time

class fheNets(object) :
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

class fhe(object) :
    def __init__ (self, N, M) :
        self.N = N
        self.M = M
        self.D = np.random.randint(2, size = (N,N))
        self.E = np.linalg.inv(self.D)                

    def encryption(self, X) :
        noise = np.random.randint(int(1000), size=len(X))
        return (self.M * np.dot(self.E, X) + np.dot(self.E, noise))


    def decryption(self, C) :
        return (np.round(np.dot(self.D, C)/self.M))

    def getM(self) :
        return self.M

    def getE(self) :
        return self.E

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
    epoch = 400       
    net = fheNets(784, 10, 0.01)
    C = []
    en = fhe(784, 12345654321)        
    
    #加密所有的数据
    start = time.clock()
    for x in X :
        C.append(en.encryption(x))
    end = time.clock()
    print("encryption time = " + str(end - start))

    #在密文上进行训练
    net.training(C,Y,epoch)

    #根据秘钥更新网络
    net.encode_w(en.getM(), en.getE())

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
    save.to_csv('submission.csv',index = False,)

if __name__ == "__main__" :
    main() 