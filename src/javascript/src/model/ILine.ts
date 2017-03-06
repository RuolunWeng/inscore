///<reference path="IObject.ts"/>
///<reference path="../lib/TPoint.ts"/>

class ILine extends IObject {
    
    //protected kLineType: string;
    protected fPoint: TPoint;
    protected fWAMode: boolean;
    protected fLWidth: number
    protected fLAngle: number
    
    constructor(name: string, parent: IObject) {
        super(name, parent);
        this.fTypeString = kLineType;
        this.fPenControl.setPenWidth(1);
        this.fGetMsgHandlerMap[""] = new TGetMsgHandlerArray(this._getPoint());
    }
    
    setPoint(p: TPoint)		{ 
    	this.fPoint = p; 
    	this.fPosition.setWidth  (Math.abs(p.getX())); 
    	this.fPosition.setHeight (Math.abs(p.getY())); 
    }
     getPoint(): TPoint	        	{ return this.fPoint; }
    _getPoint(): GetArrayMethod     { return () => this.fPoint.toArray(); }

    getColorTarget(): IColor 		{ return this.fPenControl.fPenColor; };
    
    set(msg:IMessage): msgStatus {
        let status = super.set(msg);
        if (status & (msgStatus.kProcessed + msgStatus.kProcessedNoChange)) return status;
        
        if (msg.size() == 5) {
            let mode = msg.paramStr(2); 
            let a = msg.paramNum(3), b = msg.paramNum(4); 
            
            if (!mode.correct)				return msgStatus.kBadParameters;
            if (!a.correct || !b.correct) 	return msgStatus.kBadParameters;
               
            if (mode.value == "xy") {
                this.fWAMode = false;
            	this.setPoint( new TPoint(a.value, b.value) );
            }
            else if (mode.value == "wa") {
                this.fWAMode = true;
                this.fLWidth = a.value;
                this.fLAngle = b.value;
                
                if (this.fLAngle < 0) { this.fLAngle += 360; }
                if ( 180 <= Math.abs(this.fLAngle) && Math.abs(this.fLAngle) <= 360) { this.fLAngle -= 180; }                 

                let x = Math.round(a.value * Math.cos(Math.PI * this.fLAngle / 180) * 1000) / 1000;
                let y = Math.round(a.value * Math.sin(Math.PI * this.fLAngle / 180) * 1000) / 1000;
                this.setPoint( new TPoint(x, y) );
            }
            else return msgStatus.kBadParameters;
            this.newData(true);
            status = msgStatus.kProcessed;
        }
        else status = msgStatus.kBadParameters;
        return status;
    }

    getSetLine(p: Array<any>): Array<any>	{
    	return this.fWAMode ? ["wa", this.fLWidth, this.fLAngle] : ["xy", this.fPoint.getX(), this.fPoint.getY()];
    }

    getSet(): IMessage	{
    	return new IMessage(this.getOSCAddress(), this.getSetLine([kset_SetMethod, this.fTypeString])); 
    }
}