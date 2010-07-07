#! /usr/bin/env ruby

{% include licence|append:".rb" %}

require 'Qt4'

class {{ className }} {% if baseClass %} < {{ baseClass.type }}{% endif %}

    {% for property in properties %}
    {% if property.readonly %}
    attr_reader :{{property.name}}
    {% else %}
    attr_accessor :{{property.name}}
    {% endif %}
    {% endfor %}

    def initialize
        super
    end

    {% with "true" as default %}{% for method in methods %}
    def {{ method.name }}{% if method.args %}({% include "args.rb" %}){% endif %}
    end
    {% endfor %}{% endwith %}

end

