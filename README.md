> https://github.com/jnthas/clockwise  
> this was forked from version 1.1.0 of Clockwise  
> not planning to upgrade to newer versions 

# changes made to v1.1.0
- major changes are in Clockface.cpp
- retrieve stock data from yahoo finance every 5 minutes
- added card to web config page to select the tickers to display
    
![image](https://github.com/slcwang/clockwise/assets/131223926/dd3112b2-367b-47aa-b48f-e0122b1610aa)  
  
![image](https://github.com/slcwang/clockwise/assets/131223926/55b12bbc-3bee-4d15-8a86-8f702c53eee2)


# future developments (maybe)
- pass web config data from main.cpp to Clockface.cpp with json.  
  to avoid repetitive code in Clockface.cpp.  
- yahoo finance API is free but can be delayed up to 20 mins.  
  planning to use other APIs that is realtime
  looked into FUTU open API, but it requires a client software (FutuOpenD) to run on a server and there is no naked HTTP API
   
