
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



var UpperFilter = function(input)
{
  // Duck-cast it to a string
  input = input + "";
  return input.toUpperCase();
};
UpperFilter.filterName = "upper";
Library.addFilter("UpperFilter");



var JoinFilter = function(input, filterArgument)
{
  return input.join(filterArgument);
};
JoinFilter.filterName = "join";
Library.addFilter("JoinFilter");


