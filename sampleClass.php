<?php 
    Class Feature{
        public $ID;
        public $SiteID;
        public $Name; 
        public $FeatureType;
        public $Diameter;
        public $DiameterMin;
        public $DiameterMax;
        public $FeatureQuantity;
        public $LengthMin;
        public $WidthMin;
        public $WidthMax;
        public $Depth;
        public $Shape; 
        private $conn;
        private $table_name = "Features";
    }
?>