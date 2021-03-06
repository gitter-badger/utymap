﻿using System;
using Assets.UtymapLib;
using Assets.UtymapLib.Core;
using Assets.UtymapLib.Infrastructure.Config;
using Assets.UtymapLib.Infrastructure.Dependencies;
using Assets.UtymapLib.Infrastructure.Diagnostic;
using Assets.UtymapLib.Infrastructure.IO;

namespace UtymapLib.Tests.Helpers
{
    internal static class TestHelper
    {
        #region Constants values

        public const string IntegrationTestCategory = "Integration";

        public static GeoCoordinate WorldZeroPoint = new GeoCoordinate(52.5317429, 13.3871987);

        public const string TestAssetsFolder = @"../../../Assets/Resources";
        public const string ConfigTestRootFile = TestAssetsFolder + @"/Config/test.json";
        public const string BerlinXmlData = TestAssetsFolder + @"/Osm/berlin.osm.xml";
        public const string BerlinPbfData = TestAssetsFolder + @"/Osm/berlin.osm.pbf";
        public const string NmeaFilePath = TestAssetsFolder + @"/Nmea/invalidenstrasse_borsigstrasse.nme";
        public const string DefaultMapCss = TestAssetsFolder + @"/MapCss/default/default.mapcss";

        #endregion

        public static JsonConfigSection GetJsonConfig(string configPath)
        {
            return new JsonConfigSection
                (new FileSystemService(new PathResolver(), new DefaultTrace()).ReadText(configPath));
        }

        public static CompositionRoot GetCompositionRoot(GeoCoordinate worldZeroPoint)
        {
            return GetCompositionRoot(worldZeroPoint, (container, section) => { });
        }

        public static CompositionRoot GetCompositionRoot(GeoCoordinate worldZeroPoint,
            Action<IContainer, IConfigSection> action)
        {
            // create default container which should not be exposed outside
            // to avoid Service Locator pattern.
            IContainer container = new Container();

            // create default application configuration
            var config = ConfigBuilder.GetDefault()
                .Build();

            // initialize services
            return new CompositionRoot(container, config)
                .RegisterAction((c, _) => c.Register(Component.For<ITrace>().Use<ConsoleTrace>()))
                .RegisterAction((c, _) => c.Register(Component.For<IPathResolver>().Use<TestPathResolver>()))
                .RegisterAction((c, _) => c.Register(Component.For<Stylesheet>().Use<Stylesheet>(DefaultMapCss)))
                .RegisterAction((c, _) => c.Register(Component.For<IProjection>().Use<CartesianProjection>(worldZeroPoint)))
                .RegisterAction(action)
                .Setup();
        }
    }
}
