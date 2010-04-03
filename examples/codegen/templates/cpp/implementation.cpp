{% include licence|append:".h" %}

#include "{{ className|lower }}.h"{% if pimpl %}
#include "{{ className|lower }}_p.h"{% endif %}

{% for property in properties %}{{ property.type }} {{ className }}::{{ property.name }}() const
{{% if pimpl %}
  Q_D(const {{ className }});{% endif %}
  return {% if pimpl %}d->{% endif %}m_{{ property.name }};
}{% if not property.readonly %}

void {{ className }}::{{ property.name|to_write }}({{ property.type|to_arg }}{{ property.name }})
{{% if pimpl %}
  Q_D({{ className }});{% endif %}
  {% if pimpl %}d->{% endif %}m_{{ property.name }} = {{ property.name }};
}{% endif %}

{% endfor %}{% comment %} Don't put default args in implementation methods.
{% endcomment %}{% with false as default %}{% for method in methods %}{% ifnotequal method.accessType "signals" %}{% comment %}
{% endcomment %}{{ method.type }} {{ className }}::{{ method.name }}({% include "args.h" %}) {% if method.const %}const{% endif %}
{

}
{% endifnotequal %}
{% endfor %}{% endwith %}
#include "{{ className|lower }}.moc"
