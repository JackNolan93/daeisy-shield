namespace JackDsp
{

class ControlRamp
{
public:
    ControlRamp() = default;
    ~ControlRamp() = default;

    void setBlockSize (int blocksize)
    {
        _blockSize = blocksize; 
    }

    void setValue (float value)
    {
        _targetVal = value;
        _inc = (_targetVal - _currentVal) / _blockSize;
    }

    void setValueImmediate (float value)
    {
        _currentVal = value;
        _targetVal = value;
        _inc = 0;
    }

    bool isRamping ()
    {
        return _targetVal != _currentVal;
    }

    void tick ()
    {
        if (_inc != 0)
        {
            _currentVal += _inc; 
            
            if (_inc * (_currentVal - _targetVal) >= 0)
            {
                setValueImmediate (_targetVal);
                return;
            }
        }
    }

    void tickMultiple (int numTicks)
    {   
        if (_inc != 0)
        {
             _currentVal += _inc * numTicks;

            if (_inc * (_currentVal - _targetVal) >= 0)
            {
                setValueImmediate (_targetVal);
                return;
            }
        }
    }

    float getValue () { return _currentVal; }

private:

    float _currentVal = 0;
    float _targetVal = 0;
    float _inc = 0;
    int _blockSize = 0;

};

}
