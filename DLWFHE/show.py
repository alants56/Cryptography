import pandas 
import matplotlib.pyplot as plt
import numpy as np

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

def getdata() :
    Y, X = load_train_data("train.csv")
    data = []
    c = 0
    for i in range (len(Y)) :
        if c == 10 :
            break;
        if Y[i] == c :
            data.append(X[i])  
            c += 1

    return np.array(data)         

def main() :
    en = fhe(784, 12345654321)
    data = getdata()
    fig = plt.figure()
    for i in range(10) :
        x = data[i]
        ax = fig.add_subplot(3,10, i+1)
        ax.imshow(x.reshape(28, 28), cmap=plt.cm.gray)
        c = en.encryption(x.flatten())
        ax = fig.add_subplot(3,10,10+i+1)
        ax.imshow(c.reshape(28,28), cmap=plt.cm.gray)
        m = en.decryption(c.flatten())
        ax = fig.add_subplot(3,10,20+i+1)
        ax.imshow(m.reshape(28,28), cmap=plt.cm.gray)
    plt.show() 

if __name__ == '__main__':
    main()