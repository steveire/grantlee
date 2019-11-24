
function GetRatioNode(dob, salary, contextName)
{
  this.dob = dob;
  this.salary = salary;
  this.contextName = contextName;

  this.render = function( context )
  {
    var dob = this.dob.resolve(context);
    var salary = this.salary.resolve(context);
    var today = new Date();
    var ageMS = today - dob;
    var ageYears = ageMS / (1000 * 60 * 60 * 24 * 365);
    var ratio = salary / ageYears;
    context.insert(this.contextName, ratio);
  }
}

function GetRatioNodeFactory( tagContent, parser )
{
  var tagArgs = AbstractNodeFactory.smartSplit(tagContent);
  var node = new Node("GetRatioNode", FilterExpression(tagArgs[1]), FilterExpression(tagArgs[2]), tagArgs[4]);
  return node;
}
Library.addFactory("GetRatioNodeFactory", "get_ratio");
