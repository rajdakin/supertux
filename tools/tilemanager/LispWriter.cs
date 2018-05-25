//  $Id$
using System;
using System.IO;
using System.Collections;

public class LispWriter {
    private readonly TextWriter stream;
    private int IndentDepth;
    private Stack lists = new Stack();

    public LispWriter(TextWriter stream) {
        this.stream = stream;
    }

    public void WriteComment(string comment) {
        stream.WriteLine("; " + comment);
    }

    public void StartList(string name) {
        indent();
        stream.WriteLine("(" + name);
        IndentDepth += 2;
        lists.Push(name);
    }

    public void EndList(string name) {
        if(lists.Count == 0)
            throw new Exception("Trying to close list while none is open");
        string back = (string) lists.Pop();
        if(name != back)
            throw new Exception(
                    String.Format("Trying to close {0} which is not open", name));

        IndentDepth -= 2;
        indent();
        stream.WriteLine(")");
    }

    public void Write(string name, object value) {
        indent();
        stream.Write("(" + name);
        IEnumerable enumerableValue = value as IEnumerable;
        if(value is string) {
            stream.Write(" \"" + value + "\"");
        } else if(enumerableValue != null) {
            foreach(object o in enumerableValue) {
                stream.Write(" ");
                WriteValue(o);
            }
        } else {
            stream.Write(" ");
            WriteValue(value);
        }
        stream.WriteLine(")");
    }

    private void WriteValue(object val) {
        if(val is bool) {
            stream.Write((bool) val ? "#t" : "#f");
        } else if(val is int || val is float) {
            stream.Write(val.ToString());
        } else {
            stream.Write("\"" + val + "\"");
        }
    }

    public void WriteVerbatimLine(string line) {
        indent();
        stream.WriteLine(line);
    }

    private void indent() {
        for(int i = 0; i < IndentDepth; ++i)
            stream.Write(" ");
    }
}
