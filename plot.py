import plotly.plotly as py
import plotly.graph_objs as go

nIntervals = 20
nAttribute = 14
intervals = []
posValue = []
negValue = []
minValue = -100
maxValue =  20
step = float((maxValue-minValue)/nIntervals)
start = True
globalMax = 0
globalMin = 100

for x in range(0, nIntervals):
    intervals.append(minValue + x*step)
    posValue.append(0)
    negValue.append(0)

f = open('Datos', 'r')
for line in f:
    a = line.split(",")
    
    if (a[nAttribute] == "?"):
        continue
    
    if (start):
        #print (a[nAttribute])
        #print float(a[nAttribute])
        
        globalMax = float(a[nAttribute])
        globalMin = float(a[nAttribute])
        start = False
        
    
    if (a[nAttribute] < globalMin):
        globalMin = a[nAttribute]
    
    if (a[nAttribute] > globalMax):
        globalMax = a[nAttribute]
    
    for x in range(0, nIntervals):  
        #print float( a[nAttribute] )
        if ( ( float( a[nAttribute] ) >= minValue + x*step ) and \
           ( float( a[nAttribute] ) < minValue + (x + 1)*step ) ):
            
            if (a[15] == "+\n"):
                posValue[x] += 1
            
            if (a[15] == "-\n"):
                negValue[x] += 1
            
            break
    
print "MIN: " + str(globalMin) + " | MAX: " + str(globalMax)
#print negValue
#print posValue
f.closed
        
positiveBar = go.Bar(
    x = intervals,
    y = posValue,
    name = 'Positivo' 
)

negativeBar = go.Bar(
    x = intervals,
    y = negValue,
    name = 'Negativo' 
)

data = [positiveBar, negativeBar]

layout = go.Layout(
    title = "Atributo #" + str(nAttribute + 1),
    yaxis=dict(
        title='Numero de Casos',
    ),
    xaxis=dict(
        title='Valor',
    ),
    barmode = 'group'
)

fig = go.Figure(data=data, layout=layout)
plot_url = py.plot(fig, filename='ESTE')
