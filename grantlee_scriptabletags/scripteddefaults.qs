
function IfNode(truthVariable, trueList, falseList){
  this.truthVariable = truthVariable;
  this.trueList = trueList;
  this.falseList = falseList;
  this.render = function(context) {
    if (this.truthVariable.isTrue(context))
    {
      return context.render(this.trueList);
    } else {
      return context.render(this.falseList);
    }
  };
};

var IfNodeFactory = function(tagContent, parser, parent)
{
  var tagArgs = AbstractNodeFactory.smartSplit(tagContent);
  var truthVariable = FilterExpression(tagArgs[1]);
  var falseList;
  var trueList = parser.parse(["else", "endif2"], parent);
  if (parser.nextToken().content == "else")
  {
    falseList =  parser.parse("endif2", parent);
    parser.deleteNextToken();
  }
  return new Node("IfNode", truthVariable, trueList, falseList);
};
IfNodeFactory.tagName = "if2";
Library.addFactory("IfNodeFactory");

function IfEqualNode(expression1, expression2, trueList, falseList){
  this.expression1 = expression1;
  this.expression2 = expression2;
  this.trueList = trueList;
  this.falseList = falseList;
  this.render = function(context)
  {
    if (expression1.equals(expression2 , context))
    {
      return context.render(this.trueList);
    } else {
      return context.render(this.falseList);
    }
  }
}

IfEqualNodeFactory = function(tagContent, parser, parent)
{
  var tagArgs = AbstractNodeFactory.smartSplit(tagContent);
  var expression1 = FilterExpression(tagArgs[1], parser);
  var expression2 = FilterExpression(tagArgs[2], parser);
  var falseList;
  var trueList = parser.parse(["else", "endifequal2"], parent);
  if (parser.nextToken().content == "else")
  {
    falseList =  parser.parse("endifequal2", parent);
    parser.deleteNextToken();
  }
  return new Node("IfEqualNode", expression1, expression2, trueList, falseList);
}
IfEqualNodeFactory.tagName = "ifequal2";
Library.addFactory("IfEqualNodeFactory");

var SafeFilter = function(input)
{
  return mark_safe(input.rawString());
};
SafeFilter.filterName = "safe2";
Library.addFilter("SafeFilter");

var UpperFilter = function(input)
{
  // Duck-cast it to a string
  input = input.rawString(); // + "";
  return input.toUpperCase();
};
UpperFilter.filterName = "upper";
Library.addFilter("UpperFilter");

var JoinFilter = function(input, filterArgument)
{
  return mark_safe(input.join(filterArgument.rawString()));
};
JoinFilter.filterName = "join2";
Library.addFilter("JoinFilter");
