<xsl:stylesheet version="2.0" 
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:ecolmod="http://www.ecolmod.org">
<xsl:output indent="yes" encoding="ISO-8859-1"/>

<!-- Missing parameters -->
<xsl:variable name="UwindExponent" select="0.8"/>

<!-- Helpers -->
<xsl:variable name="colon" select="codepoints-to-string(58)"/>
<xsl:variable name="test-weather-file" select="'input/sel_dk.txt'"/> 

<!-- Call templates -->
<xsl:template name="float-with-period">
  <xsl:param name="value"/>
  <xsl:choose>
    <xsl:when test="contains(string($value), '.')">
      <xsl:attribute name="value">
        <xsl:value-of select="$value"/>
      </xsl:attribute>
    </xsl:when>
    <xsl:otherwise>
      <xsl:attribute name="value">
        <xsl:value-of select="concat($value, '.0')"/>
      </xsl:attribute>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="pct-value">
  <xsl:param name="value"/>
  <xsl:call-template name="float-with-period">
    <xsl:with-param name="value" select="number(replace($value, ',', '.')) div 100"/>
  </xsl:call-template>
</xsl:template>

<xsl:template name="time-value">
  <xsl:param name="value"/>
  <xsl:attribute name="value">
    <xsl:value-of select="concat(substring($value, 1, 2), $colon, substring($value, 3, 2), $colon, '00')"/>
  </xsl:attribute>
</xsl:template>

<xsl:function name="ecolmod:generateXPath" >
  <xsl:param name="pNode"/> <!--as="node()"/>-->

  <xsl:for-each select="$pNode/ancestor::*">    
    <xsl:value-of select="concat('/', name())" />    
  </xsl:for-each>    
    <xsl:value-of select="concat('/', name($pNode[1]))" />     
</xsl:function>

<xsl:template name="extract-cover">
  <xsl:param name="cover"/>
  <xsl:variable name="swReflectivitySrc" select="$cover/PaneReflection" as="node()"/>
  <xsl:variable name="swReflectivityValue" select="number(replace($swReflectivitySrc, ',', '.'))"/>
  <xsl:variable name="swTransmissivitySrc" select="$cover/PaneTransmission" as="node()"/>
  <xsl:variable name="swTransmissivityValue" select="number(replace($swTransmissivitySrc, ',', '.'))"/>
  <xsl:variable name="lwReflectivitySrc" select="$cover/PaneLwReflection" as="node()"/>
  <xsl:variable name="lwReflectivityValue" select="number(replace($lwReflectivitySrc, ',', '.'))"/>
  <xsl:variable name="lwTransmissivitySrc" select="$cover/PaneLwTransmission" as="node()"/>
  <xsl:variable name="lwTransmissivityValue" select="number(replace($lwTransmissivitySrc, ',', '.'))"/>
  <xsl:variable name="UwindMinimumSrc" select="$cover/PaneUValue" as="node()"/>
  <xsl:variable name="UwindMinimumValue" select="number(replace($UwindMinimumSrc, ',', '.'))"/>
  <xsl:variable name="UwindSlopeSrc" select="VG_SETUP/Greenhouse/UwindSlope" as="node()"/>
  <xsl:variable name="UwindSlopeValue" select="number(replace($UwindSlopeSrc, ',', '.'))"/>

  <box class="vg::Cover" name="cover">
    <port name="swReflectivityTop">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($swReflectivitySrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$swReflectivityValue"/>
      </xsl:attribute>
    </port>
    <port name="swReflectivityBottom">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($swReflectivitySrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$swReflectivityValue"/>
      </xsl:attribute>
    </port>
    <port name="swTransmissivityTop">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($swTransmissivitySrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$swTransmissivityValue"/>
      </xsl:attribute>
    </port>
    <port name="swTransmissivityBottom">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($swTransmissivitySrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$swTransmissivityValue"/>
      </xsl:attribute>
    </port>

    <port name="lwReflectivityTop">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($lwReflectivitySrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$lwReflectivityValue"/>
      </xsl:attribute>
    </port>
    <port name="lwReflectivityBottom">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($lwReflectivitySrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$lwReflectivityValue"/>
      </xsl:attribute>
    </port>
    <port name="lwTransmissivityTop">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($lwTransmissivitySrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$lwTransmissivityValue"/>
      </xsl:attribute>
    </port>
    <port name="lwTransmissivityBottom">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($lwTransmissivitySrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$lwTransmissivityValue"/>
      </xsl:attribute>
    </port>
    
    <port name="UwindMinimum">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($UwindMinimumSrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$UwindMinimumValue"/>
      </xsl:attribute>
    </port>
    <port name="UwindSlope">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($UwindSlopeSrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$UwindSlopeValue"/>
      </xsl:attribute>
    </port>
    <port name="UwindExponent" source="Fixed">
      <xsl:attribute name="value">
        <xsl:value-of select="$UwindExponent"/>
      </xsl:attribute>
    </port>
  </box>
</xsl:template>

<xsl:template name="extract-screen">
  <xsl:param name="position"/>
  <xsl:if test="lower-case(Product)!='none'">
    <xsl:variable name="layer" select="@layer"/>
    <xsl:variable name="reflectivityTopSrc" select="ReflectionOutwards" as="node()"/>
    <xsl:variable name="reflectivityTopValue" select="number(replace($reflectivityTopSrc, ',', '.'))"/>
    <xsl:variable name="reflectivityBottomSrc" select="ReflectionInwards" as="node()"/>
    <xsl:variable name="reflectivityBottomValue" select="number(replace($reflectivityBottomSrc, ',', '.'))"/>
    <xsl:variable name="transmissivitySrc" select="Transmission" as="node()"/>
    <xsl:variable name="transmissivityValue" select="number(replace($transmissivitySrc, ',', '.'))"/>
    <xsl:variable name="USrc" select="U"/>
    <xsl:variable name="UValue" select="number(replace($USrc, ',', '.'))"/>
    <xsl:variable name="transmissivityAirSrc" select="Porosity" as="node()"/>
    <xsl:variable name="transmissivityAirValue" select="number(replace($transmissivityAirSrc, ',', '.'))"/>
          
    <box name="MISSING">
    
      <xsl:choose>  
        <xsl:when test="$position=1 or $position=2">
          <xsl:attribute name="class">
            <xsl:value-of select="'vg::ScreenRoof'"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="class">
            <xsl:value-of select="'vg::ScreenWall'"/>
          </xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>  
      
      <xsl:attribute name="name">
        <xsl:value-of select="concat('layer', $layer)"/>
      </xsl:attribute>
      
      <port name="swTransmissivityTop">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($transmissivitySrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$transmissivityValue"/>
        </xsl:attribute>
      </port>
      <port name="swTransmissivityBottom">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($transmissivitySrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$transmissivityValue"/>
        </xsl:attribute>
      </port>
      <port name="swReflectivityTop">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($reflectivityTopSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$reflectivityTopValue"/>
        </xsl:attribute>
      </port>
      <port name="swReflectivityBottom">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($reflectivityBottomSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$reflectivityBottomValue"/>
        </xsl:attribute>
      </port>
      <port name="lwTransmissivityTop">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($transmissivitySrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$transmissivityValue"/>
        </xsl:attribute>
      </port>
      <port name="lwTransmissivityBottom">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($transmissivitySrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$transmissivityValue"/>
        </xsl:attribute>
      </port>

      <port name="lwReflectivityTop">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($reflectivityTopSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$reflectivityTopValue"/>
        </xsl:attribute>
      </port>
      <port name="lwReflectivityBottom">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($reflectivityBottomSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$reflectivityBottomValue"/>
        </xsl:attribute>
      </port>

      <port name="Utop">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($USrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$UValue"/>
        </xsl:attribute>
      </port>
      <port name="Ubottom">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($USrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$UValue"/>
        </xsl:attribute>
      </port>
      
      <port name="transmissivityAir">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($transmissivityAirSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$transmissivityAirValue"/>
        </xsl:attribute>
      </port>
      
      <port name="state">
        <xsl:attribute name="ref">
          <xsl:value-of select="concat('actuators/screens/layer', $layer, '[value]')"/>
        </xsl:attribute>
      </port>
    </box>
  </xsl:if>
</xsl:template>

<xsl:template name="extract-screens">
  <xsl:param name="position"/>
  <xsl:for-each select=".[@layer='1']">
    <xsl:call-template name="extract-screen">
      <xsl:with-param name="position" select="$position"/>
    </xsl:call-template>
  </xsl:for-each>
  <xsl:for-each select=".[@layer='2']">
    <xsl:call-template name="extract-screen">
      <xsl:with-param name="position" select="$position"/>
    </xsl:call-template>
  </xsl:for-each>
  <xsl:for-each select=".[@layer='3']">
    <xsl:call-template name="extract-screen">
      <xsl:with-param name="position" select="$position"/>
    </xsl:call-template>
  </xsl:for-each>
</xsl:template>

<xsl:template name="extract-shelter-face">
  <xsl:param name="position"/>  

  <box class="vg::ShelterFaceArea" name="area"/>
  <xsl:call-template name="extract-cover">
    <xsl:with-param name="cover" select="VG_SETUP/Greenhouse/Panes/Pane[@position=$position]"/>
  </xsl:call-template>
  <box class="vg::Screens" name="screens">
    <xsl:for-each select="VG_SETUP/Greenhouse/Screens/Screen[@position=$position]">
      <xsl:call-template name="extract-screens">
        <xsl:with-param name="position" select="$position"/>
      </xsl:call-template>
    </xsl:for-each>
  </box>
</xsl:template>

<xsl:template name="extract-vent">
  <xsl:param name="position"/>
  <xsl:variable name="ventName" select="'vent'"/>
  <xsl:variable name="lengthSrc" select="VG_SETUP/Greenhouse/Vents/Vent[@position=$position]/Length"/>
  <xsl:variable name="lengthValue" select="number(replace($lengthSrc, ',', '.'))"/>
  <xsl:variable name="widthSrc" select="VG_SETUP/Greenhouse/Vents/Vent[@position=$position]/Width"/>
  <xsl:variable name="widthValue" select="number(replace($widthSrc, ',', '.'))"/>
  <xsl:variable name="numberSrc" select="VG_SETUP/Greenhouse/Vents/Vent[@position=$position]/Number"/>
  <xsl:variable name="numberValue" select="number(replace($numberSrc, ',', '.'))"/>
  <xsl:variable name="transmissivitySrc" select="VG_SETUP/Greenhouse/Vents/Vent[@position=$position]/VentTransmissivity"/>
  <xsl:variable name="transmissivityValue" select="number(replace($transmissivitySrc, ',', '.'))"/>

  <box class="vg::Vent" name="vent">
    <xsl:attribute name="name">
      <xsl:value-of select="$ventName"/>
    </xsl:attribute>
    <port name="length">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($lengthSrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$lengthValue"/>
      </xsl:attribute>
    </port>
    <port name="width">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($widthSrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$widthValue"/>
      </xsl:attribute>
    </port>
    <port name="number">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($numberSrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$numberValue"/>
      </xsl:attribute>
    </port>
    <port name="transmissivity">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($transmissivitySrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$transmissivityValue"/>
      </xsl:attribute>
    </port>
  </box>
</xsl:template>

<xsl:template name="extract-setpoints">
  <xsl:param name="climateSetpointName"/>
  <xsl:param name="encodeClassName" select="''"/>
  <xsl:param name="initialSignalIsZero" select="'no'"/>
  
  <xsl:variable name="setpoint-index"  select="VG_SETUP/Greenhouse/Climate/Setpoint/*[name()=$climateSetpointName]"/>
  
  <port name="initialSignal" value="0">  
    <xsl:if test="count($setpoint-index)>0 and $initialSignalIsZero='no'">
      <xsl:attribute name="value">
        <xsl:value-of select="$setpoint-index[1]"/>
      </xsl:attribute>
    </xsl:if>
  </port>
  
  <port name="reverseOrder" value="TRUE"/>

  <xsl:for-each select="$setpoint-index">
    <xsl:variable name="beginDateValue" select="@FromDate"/>
    <xsl:variable name="endDateValue" select="@ToDate"/>
    <xsl:variable name="beginTimeValue" select="@FromTime"/>
    <xsl:variable name="endTimeValue" select="@ToTime"/>
    <xsl:variable name="signalSrc" select="."/>
    <xsl:variable name="signalValue" select="number(replace($signalSrc, ',', '.'))"/>
    <xsl:variable name="signalCode" select="$signalSrc"/>
    
    <box class="DateTimeSignal">
      <port name="beginDate">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath(.)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$beginDateValue"/>
        </xsl:attribute>
      </port>
      <port name="endDate">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath(.)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$endDateValue"/>
        </xsl:attribute>
      </port>
      <port name="beginTime">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath(.)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$beginTimeValue"/>
        </xsl:attribute>
      </port>
      <port name="endTime">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath(.)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$endTimeValue"/>
        </xsl:attribute>
      </port>
      <!-- Use signal value or refer to object of encode class? -->
      <port name="signalInside">
        <xsl:choose>
          <xsl:when test="string-length($encodeClassName)=0">
            <xsl:attribute name="value">
              <xsl:value-of select="$signalValue"/>
            </xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="ref">
              <xsl:value-of select="'./code[value]'"/>
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
      </port>
      <!-- Create box of encode class -->
      <xsl:if test="string-length($encodeClassName)!=0">
        <box name="code">
          <xsl:attribute name="class">
            <xsl:value-of select="$encodeClassName"/>
          </xsl:attribute>
          <port name="formula">
            <xsl:attribute name="value">
              <xsl:value-of select="$signalCode"/>
            </xsl:attribute>
          </port>
        </box>
      </xsl:if>
    </box>
  </xsl:for-each>
</xsl:template>


<xsl:template name="extract-growth-light">
  <xsl:variable name="index" select="@index"/>
  <xsl:variable name="growthLightName" select="concat('growthLight', $index)"/>
  <!-- <xsl:variable name="typeSrc" select="LampType" as="node()"/> -->
  <!-- <xsl:variable name="typeValue" select="LampType"/> -->
  <xsl:variable name="intensitySrc" select="LightCapacityPerSqm"/>
  <xsl:variable name="intensityValue" select="number(replace($intensitySrc, ',', '.'))"/>
  <xsl:variable name="parPhotonCoefSrc" select="MicromolParPerWatt"/>
  <xsl:variable name="parPhotonCoefValue" select="number(replace($parPhotonCoefSrc, ',', '.'))"/>
  <xsl:variable name="ageCorrectedEfficiencySrc" select="ageCorrectedEfficiency"/>
  <xsl:variable name="ageCorrectedEfficiencyValue" select="number(replace($ageCorrectedEfficiencySrc, ',', '.'))"/>

  <xsl:if test="$intensityValue > 0">
    <box class="vg::GrowthLight">
      <xsl:attribute name="name">
        <xsl:value-of select="$growthLightName"/>
      </xsl:attribute>
      <!-- <port name="type"> -->
        <!-- <xsl:attribute name="source"> -->
          <!-- <xsl:value-of select="ecolmod:generateXPath($typeSrc)"/> -->
        <!-- </xsl:attribute> -->
        <!-- <xsl:attribute name="value"> -->
          <!-- <xsl:value-of select="$typeValue"/> -->
        <!-- </xsl:attribute> -->
      <!-- </port>   -->
      <port name="on">
        <xsl:attribute name="ref">
          <xsl:value-of select="concat('controllers/growthLights/', $growthLightName, '[flagIsUp]')"/>
        </xsl:attribute>
      </port> 
      <port name="intensity">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($intensitySrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$intensityValue"/>
        </xsl:attribute>
      </port>
      <port name="parPhotonCoef">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($parPhotonCoefSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$parPhotonCoefValue"/>
        </xsl:attribute>
      </port>
      <port name="ageCorrectedEfficiency">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($ageCorrectedEfficiencySrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$ageCorrectedEfficiencyValue"/>
        </xsl:attribute>
      </port>
    </box>
  </xsl:if>
</xsl:template>

<xsl:template name="extract-heat-pipe">
  <xsl:variable name="index" select="@index"/>
  <xsl:variable name="pipeName" select="concat('pipe', $index)"/>
  <xsl:variable name="materialSrc" select="PipeMaterialDesc" as="node()"/>
  <xsl:variable name="materialValue" select="$materialSrc"/>
  <xsl:variable name="densitySrc" select="PipeLengthPerSqm" as="node()"/>
  <xsl:variable name="densityValue" select="number(replace($densitySrc, ',', '.'))"/>
  <xsl:variable name="diameterSrc" select="InnerDiameter" as="node()"/>
  <xsl:variable name="diameterValue" select="number(replace($diameterSrc, ',', '.'))"/>
  <xsl:variable name="flowRateSrc" select="heatPipeFlowRate" as="node()"/>
  <xsl:variable name="flowRateValue" select="number(replace($flowRateSrc, ',', '.'))"/>

  <box class="vg::Pipe">
    <xsl:attribute name="name">
      <xsl:value-of select="$pipeName"/>
    </xsl:attribute>
    <port name="density">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($densitySrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$densityValue"/>
      </xsl:attribute>
    </port>
    <port name="diameter">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($diameterSrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$diameterValue * 1000"/>
      </xsl:attribute>
    </port>
    <port name="flowRate">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($flowRateSrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$flowRateValue"/>
      </xsl:attribute>
    </port>
  </box>
</xsl:template>

<xsl:template name="extract-heat-exchanger">
  <xsl:variable name="index" select="@index"/>
  <xsl:variable name="heatExchangerName" select="concat('heatExchanger', $index)"/>
  <xsl:variable name="numberSrc" select="Number" as="node()"/>
  <xsl:variable name="numberValue" select="number(replace($numberSrc, ',', '.'))"/>
  <xsl:variable name="efficiencySrc" select="Efficiency" as="node()"/>
  <xsl:variable name="efficiencyValue" select="number(replace($efficiencySrc, ',', '.'))"/>
  <xsl:variable name="maxFlowRateSrc" select="MaxFlowRate" as="node()"/>
  <xsl:variable name="maxFlowRateValue" select="number(replace($maxFlowRateSrc, ',', '.'))"/>
  <xsl:variable name="maxPowerUserHeatExchangeSrc" select="MaxLoad" as="node()"/>
  <xsl:variable name="maxPowerUserHeatExchangeValue" select="number(replace($maxPowerUserHeatExchangeSrc, ',', '.'))"/>
  <xsl:variable name="maxPowerUserParasiticSrc" select="MaxParasitLoad" as="node()"/>
  <xsl:variable name="maxPowerUserParasiticValue" select="number(replace($maxPowerUserParasiticSrc, ',', '.'))"/>

  <xsl:if test="$numberValue &gt; 0">
    <box class="vg::ActuatorHeatExchanger">
      <xsl:attribute name="name">
        <xsl:value-of select="$heatExchangerName"/>
      </xsl:attribute>
      <port name="number">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($numberSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$numberValue"/>
        </xsl:attribute>
      </port>
      <port name="efficiency">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($efficiencySrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$efficiencyValue"/>
        </xsl:attribute>
      </port>
      <port name="maxFlowRate">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($maxFlowRateSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$maxFlowRateValue"/>
        </xsl:attribute>
      </port>
      <port name="maxPowerUserHeatExchange">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($maxPowerUserHeatExchangeSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$maxPowerUserHeatExchangeValue"/>
        </xsl:attribute>
      </port>
      <port name="maxPowerUserParasitic">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($maxPowerUserParasiticSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$maxPowerUserParasiticValue"/>
        </xsl:attribute>
      </port>
    </box>
  </xsl:if>
</xsl:template>

<xsl:template name="extract-heat-pump">
  <xsl:variable name="index" select="@index"/>
  <xsl:variable name="heatPumpName" select="concat('heatPump', $index)"/>
  <xsl:variable name="numberSrc" select="Number" as="node()"/>
  <xsl:variable name="numberValue" select="number(replace($numberSrc, ',', '.'))"/>
  <xsl:variable name="maxCoolingPowerSrc" select="MaxCoolingLoad" as="node()"/>
  <xsl:variable name="maxCoolingPowerValue" select="number(replace($maxCoolingPowerSrc, ',', '.'))"/>
  <xsl:variable name="coolingEfficiencySrc" select="CoolingEfficiency" as="node()"/>
  <xsl:variable name="coolingEfficiencyValue" select="number(replace($coolingEfficiencySrc, ',', '.'))"/>
  <xsl:variable name="maxFlowRateSrc" select="MaxFlowRate" as="node()"/>
  <xsl:variable name="maxFlowRateValue" select="number(replace($maxFlowRateSrc, ',', '.'))"/>
  <xsl:variable name="maxPowerUserParasiticSrc" select="MaxParasitLoad" as="node()"/>
  <xsl:variable name="maxPowerUserParasiticValue" select="number(replace($maxPowerUserParasiticSrc, ',', '.'))"/>
  <xsl:variable name="coolingTemperatureSrc" select="CoolingTemperature" as="node()"/>
  <xsl:variable name="coolingTemperatureValue" select="number(replace($coolingTemperatureSrc, ',', '.'))"/>

  <xsl:if test="$numberValue &gt; 0">
    <box class="vg::ActuatorHeatPump">
      <xsl:attribute name="name">
        <xsl:value-of select="$heatPumpName"/>
      </xsl:attribute>
      <port name="number">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($numberSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$numberValue"/>
        </xsl:attribute>
      </port>
      <port name="maxCoolingPower">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($maxCoolingPowerSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$maxCoolingPowerValue"/>
        </xsl:attribute>
      </port>
      <port name="coolingEfficiency">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($coolingEfficiencySrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$coolingEfficiencyValue"/>
        </xsl:attribute>
      </port>
      <port name="maxFlowRate">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($maxFlowRateSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$maxFlowRateValue"/>
        </xsl:attribute>
      </port>
      <port name="maxPowerUserParasitic">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($maxPowerUserParasiticSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$maxPowerUserParasiticValue"/>
        </xsl:attribute>
      </port>
      <port name="coolingTemperature">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($coolingTemperatureSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$coolingTemperatureValue"/>
        </xsl:attribute>
      </port>
    </box>
  </xsl:if>
</xsl:template>

<xsl:template name="extract-humidifier">
  <xsl:variable name="index" select="@index"/>
  <xsl:variable name="humidifierName" select="concat('humidifier', $index)"/>
  <xsl:variable name="numberSrc" select="Number" as="node()"/>
  <xsl:variable name="numberValue" select="number(replace($numberSrc, ',', '.'))"/>
  <xsl:variable name="efficiencySrc" select="Efficiency" as="node()"/>
  <xsl:variable name="efficiencyValue" select="number(replace($efficiencySrc, ',', '.'))"/>
  <xsl:variable name="maxHumidificationSrc" select="MaxHumidification" as="node()"/>
  <xsl:variable name="maxHumidificationValue" select="number(replace($maxHumidificationSrc, ',', '.'))"/>
  <xsl:variable name="maxPowerUserParasiticSrc" select="MaxParasitLoad" as="node()"/>
  <xsl:variable name="maxPowerUserParasiticValue" select="number(replace($maxPowerUserParasiticSrc, ',', '.'))"/>

  <xsl:if test="$numberValue &gt; 0">
    <box class="vg::ActuatorHumidifier">
      <xsl:attribute name="name">
        <xsl:value-of select="$humidifierName"/>
      </xsl:attribute>
      <port name="efficiency">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($efficiencySrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$efficiencyValue"/>
        </xsl:attribute>
      </port>
      <port name="maxHumidification">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($maxHumidificationSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$maxHumidificationValue"/>
        </xsl:attribute>
      </port>
      <port name="maxPowerUserParasitic">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($maxPowerUserParasiticSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$maxPowerUserParasiticValue"/>
        </xsl:attribute>
      </port>
    </box>
  </xsl:if>
</xsl:template>

<xsl:template name="extract-pad-and-fan">
  <xsl:variable name="index" select="@index"/>
  <xsl:variable name="padAndFanName" select="concat('padAndFan', $index)"/>
  <xsl:variable name="numberSrc" select="Number" as="node()"/>
  <xsl:variable name="numberValue" select="number(replace($numberSrc, ',', '.'))"/>
  <xsl:variable name="efficiencySrc" select="Efficiency" as="node()"/>
  <xsl:variable name="efficiencyValue" select="number(replace($efficiencySrc, ',', '.'))"/>
  <xsl:variable name="maxFlowRateSrc" select="MaxFlowRatw"/>
  <xsl:variable name="maxFlowRateValue" select="number(replace($maxFlowRateSrc, ',', '.'))"/>
  <xsl:variable name="maxPowerUserParasiticSrc" select="MaxParasitLoad" as="node()"/>
  <xsl:variable name="maxPowerUserParasiticValue" select="number(replace($maxPowerUserParasiticSrc, ',', '.'))"/>

  <xsl:if test="$numberValue &gt; 0">
    <box class="vg::ActuatorPadAndFan">
      <xsl:attribute name="name">
        <xsl:value-of select="$padAndFanName"/>
      </xsl:attribute>
      <port name="efficiency">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$efficiencyName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($efficiencySrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$efficiencyValue"/>
        </xsl:attribute>
      </port>
      <port name="maxFlowRate">
        <xsl:attribute name="externalName">
          <xsl:value-of select="$maxFlowRateName"/>
        </xsl:attribute>
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($maxFlowRateSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$maxFlowRateValue"/>
        </xsl:attribute>
      </port>
      <port name="maxPowerUserParasitic">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($maxPowerUserParasiticSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$maxPowerUserParasiticValue"/>
        </xsl:attribute>
      </port>
    </box>
  </xsl:if>
</xsl:template>

<!-- <xsl:template name="actuator-screen"> -->
  <!-- <xsl:param name="layer"/> -->
  <!-- <xsl:variable name="screen-name" select="concat('screenLayer', $layer)"/> -->
  
  <!-- <box class="Accumulator">  -->
    <!-- <xsl:attribute name="name"> -->
      <!-- <xsl:value-of select="concat('layer', $layer)"/>  -->
    <!-- </xsl:attribute> -->
    <!-- <port name="change" ref="./controller[controlVariable]"/> -->
    <!-- <port name="minValue" value="0"/> -->
    <!-- <port name="maxValue" ref="setpoints[maxScreen]"/> -->
    <!-- <box class="ScreenCombination" name="desiredValue"> -->
      <!-- <port name="formula"> -->
        <!-- <xsl:attribute name="source"> -->
          <!-- <xsl:value-of select="concat('VG_SETUP/Greenhouse/Climate/Setpoint/', $screen-name)"/> -->
        <!-- </xsl:attribute> -->
        <!-- <xsl:attribute name="value"> -->
          <!-- <xsl:value-of select="$screen-name"/> -->
        <!-- </xsl:attribute> -->
      <!-- </port> -->
    <!-- </box> -->
    <!-- <box class="PidController" name="controller"> -->
      <!-- <port name="desiredValue"> -->
        <!-- <xsl:attribute name="ref"> -->
          <!-- <xsl:value-of select="concat('controllers/screens/', $screen-name, '[value]')"/> -->
        <!-- </xsl:attribute> -->
      <!-- </port> -->
      <!-- <port name="desiredValue" ref="../desiredValue[value]"/> -->
      <!-- <port name="sensedValue" ref="..[value]"/> -->
      <!-- <port name="Kprop" value="0.05"/> -->
    <!-- </box> -->
  <!-- </box> -->
<!-- </xsl:template> -->

  
<!-- MAIN -->

<xsl:template match="/"> <box class="Simulation" name="greenhouse">
  <port name="steps" ref="/calendar[steps]"/>
  <port name="unattended" value="TRUE"/>

  <xsl:comment> *** Calendar *** </xsl:comment>
  <xsl:variable name="beginDate" select="VG_SETUP/Description/StartTime"/>
  <xsl:variable name="endDate"   select="VG_SETUP/Description/StopTime"/>
  <xsl:variable name="timeStep"  select="VG_SETUP/Description/TimeStep"/>

  <xsl:variable name="latitudeSrc"   select="VG_SETUP/Description/Latitude" as="node()"/>
  <xsl:variable name="latitudeValue" select="number(replace($latitudeSrc, ',', '.'))"/>
  <xsl:variable name="longitudeSrc" select="VG_SETUP/Description/Longitude" as="node()"/>
  <xsl:variable name="longitudeValue" select="number(replace($longitudeSrc, ',', '.'))"/>

  <box class="Calendar" name="calendar">
    <port name="begin">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($beginDate)"/>
      </xsl:attribute>
      <xsl:attribute name="computes">
        <xsl:value-of select="concat($beginDate, ' - 1')"/>
      </xsl:attribute>
    </port>
    <port name="end">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($endDate)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="concat($endDate, 'T24:00:00')"/>
      </xsl:attribute>    </port>
    <port name="timeStep">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($timeStep)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$timeStep"/>
      </xsl:attribute>
    </port>
    <port name="timeUnit" value="m" source="Fixed"/>
    <port name="latitude">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($latitudeSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$latitudeValue"/>
        </xsl:attribute>
    </port>
    <port name="longitude">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($longitudeSrc)"/>
      </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$longitudeValue"/>
        </xsl:attribute>
    </port>
    <box class="Sun" name="sun">
    </box>
  </box>
  <xsl:comment> *** Outdoors *** </xsl:comment>
  <xsl:variable name="fileName"  select="VG_SETUP/Description/WeatherFile"/>
  <box class="vg::Outdoors" name="outdoors">
    <box class="Records" name="records">
      <port name="fileName">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($fileName)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$fileName"/>
        </xsl:attribute>
      </port>
      <port name="cycle" value="TRUE"/> <!-- implies that cycle=TRUE as well -->
    </box>
  </box>
  <xsl:comment> *** Construction *** </xsl:comment>
  <xsl:variable name="orientationSrc" select="VG_SETUP/Greenhouse/Orientation" as="node()"/>
  <xsl:variable name="orientationValue" select="number(replace($orientationSrc, ',', '.'))"/>
  <xsl:variable name="numSpansSrc" select="VG_SETUP/Greenhouse/NumberOfSpans" as="node()"/>
  <xsl:variable name="numSpansValue" select="number(replace($numSpansSrc, ',', '.'))"/>
  <xsl:variable name="spanWidthSrc" select="VG_SETUP/Greenhouse/SpanSize" as="node()"/>
  <xsl:variable name="spanWidthValue" select="number(replace($spanWidthSrc, ',', '.'))"/>
  <xsl:variable name="lengthSrc" select="VG_SETUP/Greenhouse/Length" as="node()"/>
  <xsl:variable name="lengthValue" select="number(replace($lengthSrc, ',', '.'))"/>
  <xsl:variable name="heightSrc" select="VG_SETUP/Greenhouse/TrempelHeight" as="node()"/>
  <xsl:variable name="heightValue" select="number(replace($heightSrc, ',', '.'))"/>
  <xsl:variable name="roofPitchSrc" select="VG_SETUP/Greenhouse/RoofPitch" as="node()"/>
  <xsl:variable name="roofPitchValue" select="number(replace($roofPitchSrc, ',', '.'))"/>
  <xsl:variable name="reflectionSrc" select="VG_SETUP/Greenhouse/GreenhouseReductionFactorLight" as="node()"/>
  <xsl:variable name="reflectionValue" select="number(replace($reflectionSrc, ',', '.'))"/>
  <xsl:variable name="floorReflectivitySrc" select="VG_SETUP/Greenhouse/floor-reflectance" as="node()"/>
  <xsl:variable name="floorReflectivityValue" select="number(replace($floorReflectivitySrc, ',', '.'))"/>
  <xsl:variable name="floorUtopSrc" select="VG_SETUP/Greenhouse/floor-Uindoors" as="node()"/>
  <xsl:variable name="floorUtopValue" select="number(replace($floorUtopSrc, ',', '.'))"/>
  <xsl:variable name="floorUbottomSrc" select="VG_SETUP/Greenhouse/floor-Usoil" as="node()"/>
  <xsl:variable name="floorUbottomValue" select="number(replace($floorUbottomSrc, ',', '.'))"/>
  <xsl:variable name="floorHeatCapacitySrc" select="VG_SETUP/Greenhouse/floor-heatCapacity" as="node()"/>
  <xsl:variable name="floorHeatCapacityValue" select="number(replace($floorHeatCapacitySrc, ',', '.'))"/>

  <box name="construction">
    <box class="vg::Geometry" name="geometry">
      <port name="orientation">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($orientationSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$orientationValue"/>
        </xsl:attribute>
      </port>
      <port name="numSpans">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($numSpansSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$numSpansValue"/>
        </xsl:attribute>
      </port>
      <port name="spanWidth">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($spanWidthSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$spanWidthValue"/>
        </xsl:attribute>
      </port>
      <port name="length">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($lengthSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$lengthValue"/>
        </xsl:attribute>
      </port>
      <port name="height">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($heightSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$heightValue"/>
        </xsl:attribute>
      </port>
      <port name="roofPitch">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($roofPitchSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$roofPitchValue"/>
        </xsl:attribute>
      </port>
      <port name="reflection">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($reflectionSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$reflectionValue"/>
        </xsl:attribute>
      </port>
    </box>

    <box class="vg::Shelter" name="shelter">
      <xsl:variable name="screenAirExponentSrc" select="VG_SETUP/Greenhouse/screenAirExponent" as="node()"/>
      <xsl:variable name="screenAirExponentValue" select="number(replace($screenAirExponentSrc, ',', '.'))"/>
      <xsl:variable name="UairSrc" select="VG_SETUP/Greenhouse/screenUair" as="node()"/>
      <xsl:variable name="UairValue" select="number(replace($UairSrc, ',', '.'))"/>
      <xsl:variable name="screenUstateExponentSrc" select="VG_SETUP/Greenhouse/screenUstateExponent" as="node()"/>
      <xsl:variable name="screenUstateExponentValue" select="number(replace($screenUstateExponentSrc, ',', '.'))"/>
      <xsl:variable name="screenUventilationSlopeSrc" select="VG_SETUP/Greenhouse/screenUventilationSlope" as="node()"/>
      <xsl:variable name="screenUventilationSlopeValue" select="number(replace($screenUventilationSlopeSrc, ',', '.'))"/>
      <xsl:variable name="screenPerfectionSrc" select="VG_SETUP/Greenhouse/screenPerfection" as="node()"/>
      <xsl:variable name="screenPerfectionValue" select="number(replace($screenPerfectionSrc, ',', '.'))"/>

      <port name="screenAirExponent">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($screenAirExponentSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$screenAirExponentValue"/>
        </xsl:attribute>
      </port>
      <port name="Uair">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($UairSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$UairValue"/>
        </xsl:attribute>
      </port>
      <port name="screenUstateExponent">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($screenUstateExponentSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$screenUstateExponentValue"/>
        </xsl:attribute>
      </port>
      <port name="screenUventilationSlope">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($screenUventilationSlopeSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$screenUventilationSlopeValue"/>
        </xsl:attribute>
      </port>
      <port name="screenPerfection">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($screenPerfectionSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$screenPerfectionValue"/>
        </xsl:attribute>
      </port>
      <box class="vg::ShelterFace" name="roof1">
        <xsl:call-template name="extract-shelter-face">
          <xsl:with-param name="position" select="1"/>
        </xsl:call-template>
        <xsl:call-template name="extract-vent">
          <xsl:with-param name="position" select="1"/>
        </xsl:call-template>
      </box>
      <box class="vg::ShelterFace" name="roof2">
        <xsl:call-template name="extract-shelter-face">
          <xsl:with-param name="position" select="2"/>
        </xsl:call-template>
        <xsl:call-template name="extract-vent">
          <xsl:with-param name="position" select="2"/>
        </xsl:call-template>
      </box>
      <box class="vg::ShelterFace" name="side1">
        <xsl:call-template name="extract-shelter-face">
          <xsl:with-param name="position" select="3"/>
        </xsl:call-template>
      </box>
      <box class="vg::ShelterFace" name="side2">
        <xsl:call-template name="extract-shelter-face">
          <xsl:with-param name="position" select="4"/>
        </xsl:call-template>
      </box>
      <box class="vg::ShelterFace" name="end1">
        <xsl:call-template name="extract-shelter-face">
          <xsl:with-param name="position" select="5"/>
        </xsl:call-template>
      </box>
      <box class="vg::ShelterFace" name="end2">
        <xsl:call-template name="extract-shelter-face">
          <xsl:with-param name="position" select="6"/>
        </xsl:call-template>
      </box>
    </box>

    <box name="floor">
      <aux name="reflectivity">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($floorReflectivitySrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$floorReflectivityValue"/>
        </xsl:attribute>
      </aux>
      <aux name="Utop">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($floorUtopSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$floorUtopValue"/>
        </xsl:attribute>
      </aux>
      <aux name="Ubottom">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($floorUbottomSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$floorUbottomValue"/>
        </xsl:attribute>
      </aux>
      <aux name="heatCapacity">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($floorHeatCapacitySrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$floorHeatCapacityValue"/>
        </xsl:attribute>
      </aux>
    </box>
  </box>

  <xsl:comment> *** Status *** </xsl:comment>
  <xsl:variable name="lightSumPeriodSrc" select="VG_SETUP/Status/LightSum" as="node()"/>
  <xsl:variable name="lightSumPeriodValue" select="number(replace($lightSumPeriodSrc, ',', '.'))"/>
  <xsl:variable name="temperatureAvgSrc" select="VG_SETUP/Status/TemperatureAvg" as="node()"/>
  <xsl:variable name="temperatureAvgValue" select="number(replace($temperatureAvgSrc, ',', '.'))"/>
  <box name="status">
    <box class="RunningSumTimed" name="lightSum">
      <port name="input" ref="energyBudget[cropParFluxFromAbove]"/>
      <port name="timeUnit" value="d"/>
      <port name="timeWindow">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($lightSumPeriodSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$lightSumPeriodValue"/>
        </xsl:attribute>
      </port>
    </box>
    <box class="RunningAverageTimed" name="temperatureAvg">
      <port name="initial" value="20"/>
      <port name="input" ref="indoors/temperature[value]"/>
      <port name="timeUnit" value="d"/>
      <port name="timeWindow">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($temperatureAvgSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$temperatureAvgValue"/>
        </xsl:attribute>
      </port>
    </box>
  </box>
  <xsl:comment> *** Setpoints *** </xsl:comment>
  <box class="vg::Setpoints" name="setpoints">
    <box name="elementary">
      <box class="PrioritySignal" name="heatingTemperatureNormalRh">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'HeatingTemp'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal"  name="heatingTemperatureHighRhMargin">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'MaxHeatAddHighRH'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal"  name="heatingTemperatureLowRhMargin">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'MaxHeatAddLowRH'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal"  name="minPipeTemperature1">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'minPipeTemperature1'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal"  name="minPipeTemperature2">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'minPipeTemperature2'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal"  name="rhMax">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'MaxRelHmd'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="rhMaxBand">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'rhMaxBand'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal"  name="rhMin">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'MinRelHmd'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="rhMinBand">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'rhMinBand'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal"  name="ventilationTemperatureMargin">
        <port name="initialSignal" value="5"/>  
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'VentTemp'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="crackVentilationNormalTemperature">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'crackVentilation'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="crackVentilationTemperatureMin">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'crackVentilationTemperatureMin'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="crackVentilationTemperatureMinBand">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'crackVentilationTemperatureMinBand'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="screenCrackAtHighRh">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'crackScreenHighHumidity'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="screenCrackAtHighTemperature">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'crackScreenHighTemperature'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="screenCrackAtHighTemperatureBand">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'crackScreenHighTemperatureBand'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="co2Capacity">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'CO2Capacity'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="co2Setpoint">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'CO2Setpoint'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="co2VentilationThreshold">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'CO2VentilationThreshold'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="co2VentilationBand">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'CO2VentilationBand'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="chalk">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'ShadingAgentReduction'"/>
        </xsl:call-template>
      </box>
      <box name="growthLights">
        <box name= "growthLight1">
          <box class="PrioritySignal" name="thresholdLow">
            <xsl:call-template name="extract-setpoints">
              <xsl:with-param name="climateSetpointName" select="'AssLightOn1'"/>
            </xsl:call-template>
          </box>
          <box class="PrioritySignal" name="thresholdHigh">
            <xsl:call-template name="extract-setpoints">
              <xsl:with-param name="climateSetpointName" select="'AssLightOff1'"/>
            </xsl:call-template>
          </box>
          <box class="PrioritySignal" name="setting">
            <xsl:call-template name="extract-setpoints">
              <xsl:with-param name="climateSetpointName" select="'AssLightActive1'"/>
              <xsl:with-param name="initialSignalIsZero" select="'yes'"/>
            </xsl:call-template>
          </box>
        </box>
        <box name= "growthLight2">
          <box class="PrioritySignal" name="thresholdLow">
            <xsl:call-template name="extract-setpoints">
              <xsl:with-param name="climateSetpointName" select="'AssLightOn2'"/>
            </xsl:call-template>
          </box>
          <box class="PrioritySignal" name="thresholdHigh">
            <xsl:call-template name="extract-setpoints">
              <xsl:with-param name="climateSetpointName" select="'AssLightOff2'"/>
            </xsl:call-template>
          </box>
          <box class="PrioritySignal" name="setting">
            <xsl:call-template name="extract-setpoints">
              <xsl:with-param name="climateSetpointName" select="'AssLightActive2'"/>
              <xsl:with-param name="initialSignalIsZero" select="'yes'"/>
            </xsl:call-template>
          </box>
        </box>
        <box name= "growthLight3">
          <box class="PrioritySignal" name="thresholdLow">
            <xsl:call-template name="extract-setpoints">
              <xsl:with-param name="climateSetpointName" select="'AssLightOn3'"/>
            </xsl:call-template>
          </box>
          <box class="PrioritySignal" name="thresholdHigh">
            <xsl:call-template name="extract-setpoints">
              <xsl:with-param name="climateSetpointName" select="'AssLightOff3'"/>
            </xsl:call-template>
          </box>
          <box class="PrioritySignal" name="setting">
            <xsl:call-template name="extract-setpoints">
              <xsl:with-param name="climateSetpointName" select="'AssLightActive3'"/>
              <xsl:with-param name="initialSignalIsZero" select="'yes'"/>
            </xsl:call-template>
          </box>
        </box>
      </box>
      <box name="screens">
        <box class="PrioritySignal" name="screenEnergyThreshold1">
          <xsl:call-template name="extract-setpoints">
            <xsl:with-param name="climateSetpointName" select="'screenEnergyThreshold1'"/>
          </xsl:call-template>
        </box>
        <box class="PrioritySignal" name="screenEnergyThreshold2">
          <xsl:call-template name="extract-setpoints">
            <xsl:with-param name="climateSetpointName" select="'screenEnergyThreshold2'"/>
          </xsl:call-template>
        </box>
        <box class="PrioritySignal" name="screenEnergyThresholdBand">
          <xsl:call-template name="extract-setpoints">
            <xsl:with-param name="climateSetpointName" select="'screenEnergyThresholdBand'"/>
          </xsl:call-template>
        </box>
        <box class="PrioritySignal" name="screenShadeThreshold1">
          <xsl:call-template name="extract-setpoints">
            <xsl:with-param name="climateSetpointName" select="'screenShadeThreshold1'"/>
          </xsl:call-template>
        </box>
        <box class="PrioritySignal" name="screenShadeThreshold2">
          <xsl:call-template name="extract-setpoints">
            <xsl:with-param name="climateSetpointName" select="'screenShadeThreshold2'"/>
          </xsl:call-template>
        </box>
        <box class="PrioritySignal" name="screenShadeThresholdBand">
          <xsl:call-template name="extract-setpoints">
            <xsl:with-param name="climateSetpointName" select="'screenShadeThresholdBand'"/>
          </xsl:call-template>
        </box>
        <box class="PrioritySignal" name="screenFixed1">
          <xsl:call-template name="extract-setpoints">
            <xsl:with-param name="climateSetpointName" select="'screenFixed1'"/>
          </xsl:call-template>
        </box>
        <box class="PrioritySignal" name="screenFixed2">
          <xsl:call-template name="extract-setpoints">
            <xsl:with-param name="climateSetpointName" select="'screenFixed2'"/>
          </xsl:call-template>
        </box>
        <box class="PrioritySignal" name="screenLayer1">
          <xsl:call-template name="extract-setpoints">
            <xsl:with-param name="climateSetpointName" select="'screenActive1'"/>
            <xsl:with-param name="encodeClassName" select="'ScreenCombinationEncode'"/>
            <xsl:with-param name="initialSignalIsZero" select="'yes'"/>
          </xsl:call-template>
        </box>
        <box class="PrioritySignal" name="screenLayer2">
          <xsl:call-template name="extract-setpoints">
            <xsl:with-param name="climateSetpointName" select="'screenActive2'"/>
            <xsl:with-param name="encodeClassName" select="'ScreenCombinationEncode'"/>
            <xsl:with-param name="initialSignalIsZero" select="'yes'"/>
          </xsl:call-template>
        </box>
        <box class="PrioritySignal" name="screenLayer3">
          <xsl:call-template name="extract-setpoints">
            <xsl:with-param name="climateSetpointName" select="'screenActive3'"/>
            <xsl:with-param name="encodeClassName" select="'ScreenCombinationEncode'"/>
            <xsl:with-param name="initialSignalIsZero" select="'yes'"/>
          </xsl:call-template>
        </box>
      </box>
      <box class="PrioritySignal" name="heatExchangersOn">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'HeatExchangersOn'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="heatPumpsOn">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'HeatPumpsOn'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="humidifiersOn">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'FogsOn'"/>
        </xsl:call-template>
      </box>
      <box class="PrioritySignal" name="padAndFansOn">
        <xsl:call-template name="extract-setpoints">
          <xsl:with-param name="climateSetpointName" select="'PadAndFansOn'"/>
        </xsl:call-template>
      </box>
    </box>
  </box>
  
  <xsl:comment> *** Controllers *** </xsl:comment>
  <box class="vg::Controllers" name="controllers">
    <box name="co2Injection">
      <aux name="value" ref="./controller[controlVariable]"/>
      <box class="PidController" name="controller">
        <port name="sensedValue" ref="indoors/co2[value]"/>
        <port name="desiredValue" ref="setpoints[co2Setpoint]"/>
        <port name="Kprop" value="0.05"/>
        <port name="lookAhead" value="10"/>
      </box>
    </box>
  </box>

  <xsl:comment> *** Actuators *** </xsl:comment>
  <xsl:variable name="heatPipePropConvectionSrc" select="VG_SETUP/Greenhouse/HeatPipeConvection" as="node()"/>
  <xsl:variable name="heatPipePropConvectionValue" select="number(replace($heatPipePropConvectionSrc, ',', '.'))"/>

  <xsl:variable name="heatBuffer" select="VG_SETUP/Greenhouse/HeatBuffer"/>
  <xsl:variable name="heatBufferSizeName" select="'HeatBufferSize'"/>
  <xsl:variable name="heatBufferSizeSrc" select="$heatBuffer/Constants/Parameters[ParameterName=$heatBufferSizeName]/Value" as="node()"/>
  <xsl:variable name="heatBufferSizeValue" select="number(replace($heatBufferSizeSrc, ',', '.'))"/>
  <xsl:variable name="heatBufferCapacityName" select="'HeatBufferCapacity'"/>
  <xsl:variable name="heatBufferCapacitySrc" select="$heatBuffer/Constants/Parameters[ParameterName=$heatBufferCapacityName]/Value" as="node()"/>
  <xsl:variable name="heatBufferCapacityValue" select="number(replace($heatBufferCapacitySrc, ',', '.'))"/>

  <box class="vg::Actuators" name="actuators">
    <box class="GrowthLights" name="growthLights">
      <xsl:for-each select="VG_SETUP/Greenhouse/Lamps/Lamp">
        <xsl:call-template name="extract-growth-light"/>
      </xsl:for-each>
    </box>
    <box class="ActuatorHeatPipes" name="heating">
      <port name="propConvection">
          <xsl:attribute name="source">
            <xsl:value-of select="ecolmod:generateXPath($heatPipePropConvectionSrc)"/>
          </xsl:attribute>
          <xsl:attribute name="value">
            <xsl:value-of select="$heatPipePropConvectionValue"/>
          </xsl:attribute>
      </port>
      <box name="pipes">
        <xsl:for-each select="VG_SETUP/Greenhouse/Heatpipes/Heatpipe">
          <xsl:call-template name="extract-heat-pipe"/>
        </xsl:for-each>
      </box>
    </box>
    <box class="ActuatorVentilation" name="ventilation">
    </box>
    <box name="heatExchangers">
      <xsl:for-each select="VG_SETUP/Greenhouse/HeatExchangers/HeatExchanger">
        <xsl:call-template name="extract-heat-exchanger"/>
      </xsl:for-each>
    </box>
    <box name="heatPumps">
      <xsl:for-each select="VG_SETUP/Greenhouse/HeatPumps/HeatPump">
        <xsl:call-template name="extract-heat-pump"/>
      </xsl:for-each>
    </box>
    <box name="humidifiers">
      <xsl:for-each select="VG_SETUP/Greenhouse/Fogs/Fog">
        <xsl:call-template name="extract-humidifier"/>
      </xsl:for-each>
    </box>
    <box name="padAndFans">
      <xsl:for-each select="VG_SETUP/Greenhouse/PadAndFans/PadAndFan">
        <xsl:call-template name="extract-pad-and-fan"/>
      </xsl:for-each>
    </box>
    <box class="Accumulator" name="co2Injection">
      <port name="initial" value="0"/>
      <port name="change"   ref="controllers/co2Injection[value]"/>
      <port name="minValue"  value="0"/>
      <port name="maxValue"  ref="setpoints[co2Capacity]"/>
    </box>

    <!-- <box name="screens"> -->
      <!-- <xsl:call-template name="actuator-screen"> -->
        <!-- <xsl:with-param name="layer" select="1"/> -->
      <!-- </xsl:call-template> -->
      <!-- <xsl:call-template name="actuator-screen"> -->
        <!-- <xsl:with-param name="layer" select="2"/> -->
      <!-- </xsl:call-template> -->
      <!-- <xsl:call-template name="actuator-screen"> -->
        <!-- <xsl:with-param name="layer" select="3"/> -->
      <!-- </xsl:call-template> -->
    <!-- </box> -->
  </box>

  <xsl:comment> *** Climate variables *** </xsl:comment>
  <xsl:variable name="heatSinkSrc" select="VG_SETUP/Greenhouse/HasHeatSink" as="node()"/>
  <xsl:variable name="heatSinkValue" select="number(replace($heatSinkSrc, ',', '.'))"/>
  <xsl:variable name="leakageSrc" select="VG_SETUP/Greenhouse/leakage" as="node()"/>
  <xsl:variable name="leakageValue" select="number(replace($leakageSrc, ',', '.'))"/>
  
  <box class="vg::EnergyBudgetOptimiser" name="energyBudgetOptimiser">
    <port name="hasHeatSink">
      <xsl:attribute name="source">
        <xsl:value-of select="ecolmod:generateXPath($heatSinkSrc)"/>
      </xsl:attribute>
      <xsl:attribute name="value">
        <xsl:value-of select="$heatSinkValue"/>
      </xsl:attribute>
    </port>
    <box class="vg::EnergyBudget" name="energyBudget">
    </box>
    <box name="indoors">
      <box class="vg::IndoorsVentilation" name="ventilation">
        <box class="vg::LeakageVentilation" name="leakage">
          <port name="leakage">
            <xsl:attribute name="source">
              <xsl:value-of select="ecolmod:generateXPath($leakageSrc)"/>
            </xsl:attribute>
            <xsl:attribute name="value">
              <xsl:value-of select="$leakageValue"/>
            </xsl:attribute>
          </port>
        </box>
      </box>
      <box class="vg::IndoorsTemperature" name="temperature">
      </box>
    </box>
  </box>

  
  <box class="WaterBudget" name="waterBudget"/>
  <box class="Indoors" name="indoors"/>
  
  <xsl:comment> *** Crop *** </xsl:comment>
  <xsl:variable name="laiSrc" select="VG_SETUP/Greenhouse/Crop/LAI" as="node()"/>
  <xsl:variable name="laiValue" select="number(replace($laiSrc, ',', '.'))"/>
  <xsl:variable name="coverageSrc" select="VG_SETUP/Greenhouse/Crop/propAreaCultured" as="node()"/>
  <xsl:variable name="coverageValue" select="number(replace($coverageSrc, ',', '.'))"/>
  <xsl:variable name="gammaStarSrc" select="VG_SETUP/Greenhouse/Crop/gammaStar" as="node()"/>
  <xsl:variable name="gammaStarValue" select="number(replace($gammaStarSrc, ',', '.'))"/>
  <xsl:variable name="JmaxSrc" select="VG_SETUP/Greenhouse/Crop/Jmax" as="node()"/>
  <xsl:variable name="JmaxValue" select="number(replace($JmaxSrc, ',', '.'))"/>
  <xsl:variable name="BallBerryInterceptSrc" select="VG_SETUP/Greenhouse/Crop/BallBerryIntercept" as="node()"/>
  <xsl:variable name="BallBerryInterceptValue" select="number(replace($BallBerryInterceptSrc, ',', '.'))"/>
  <xsl:variable name="BallBerrySlopeSrc" select="VG_SETUP/Greenhouse/Crop/BallBerrySlope" as="node()"/>
  <xsl:variable name="BallBerrySlopeValue" select="number(replace($BallBerrySlopeSrc, ',', '.'))"/>
  <xsl:variable name="lightRespirationSrc" select="VG_SETUP/Greenhouse/Crop/lightRespiration" as="node()"/>
  <xsl:variable name="lightRespirationValue" select="number(replace($lightRespirationSrc, ',', '.'))"/>
  
  <box class="vg::Crop" name="crop">
    <port name="lai">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($laiSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$laiValue"/>
        </xsl:attribute>
    </port>
    <port name="coverage">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($coverageSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$coverageValue"/>
        </xsl:attribute>
    </port>
    <port name="gammaStar">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($gammaStarSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$gammaStarValue"/>
        </xsl:attribute>
    </port>
    <port name="Jmax">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($JmaxSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$JmaxValue"/>
        </xsl:attribute>
    </port>
    <port name="lightRespiration">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($lightRespirationSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$lightRespirationValue"/>
        </xsl:attribute>
    </port>
    <port name="ballBerryIntercept">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($BallBerryInterceptSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$BallBerryInterceptValue"/>
        </xsl:attribute>
    </port>
    <port name="ballBerrySlope">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($BallBerrySlopeSrc)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$BallBerrySlopeValue"/>
        </xsl:attribute>
    </port>
  </box>
  <xsl:comment> *** Budget *** </xsl:comment>
  <box class="vg::Budget" name="budget"/>
  <xsl:comment> *** Output *** </xsl:comment>
  <box class="OutputR" name="output">
    <box class="vg::Outputs" name="ports">
    </box>
    <box class="PageR">
      <port name="xAxis" value="calendar[dateTime]"/>
      <box class="PlotR">
        <port name="ncol" value="4"/>
        <port name="ports" value="output/ports[output::*]"/>
      </box>
    </box>
    <box class="OutputSelector" name="selector">
      <xsl:variable name="beginDate" select="VG_SETUP/Description/StartTime"/>
      <xsl:variable name="timeStep" select="VG_SETUP/Description/TimeStep"/>
      <port name="skipFormats" value="TRUE"/>
      <port name="useLocalDecimalChar" value="TRUE"/>
      <port name="beginDateTime">
        <xsl:attribute name="source">
          <xsl:value-of select="ecolmod:generateXPath($beginDate)"/>
        </xsl:attribute>
        <xsl:attribute name="value">
          <xsl:value-of select="$beginDate"/>
        </xsl:attribute>
      </port>
    </box>
  </box>
  </box> </xsl:template>
</xsl:stylesheet>
